/**
 * @file writer.c
 * @author Anthony Wall
 * @brief Write functionality
 * 
 * @copyright Copyright (c) 2025 Anthony Wall.
 *            All rights reserved
 * 
 */
#include <stdio.h>
#include <bricli/bricli.h>
#include "system_commands.h"

/**
  * @brief Updates the pending bytes count and transmits a VT100 backspace response.
  * @param cli Pointer to the CLI instance to use.
  */
void Bricli_Backspace(BricliHandle_t *cli)
{

    // If there is less than 2 bytes in the buffer there is nothing to be deleted.
    if (cli->PendingBytes < 2)
    {
        cli->PendingBytes = 0;
    }
    // If there is more than 2 bytes we must tell the VT100 terminal to delete a character.
    else
    {
        // Move the pending bytes value back by two characters and ensure the backsapce is nulled.
        cli->PendingBytes -= 2;
        cli->RxBuffer[cli->PendingBytes + 1] = '\0';

        // Send the backspace and the VT100 delete.
        Bricli_Write(cli, 1, "\b");
        Bricli_Write(cli, 3, BRICLI_DELETE_CHAR);
    }
}

/**
  * @brief Helper function for displaying help information.
  *
  * @param cli Pointer to a BriCLI instance.
  * @param format Format string to be used for message generation.
  *
  * @return The error code from the instance's BSP write.
  */
int Bricli_PrintHelp(BricliHandle_t *cli)
{
    return Bricli_SystemHandlerHelp(cli, 0, NULL);
}

/**
 * @brief Helper function for sending formatted messages through a CLI instance.
 *
 * @param cli Pointer to a BriCLI instance.
 * @param format Format string to be used for message generation.
 *
 * @return The error code from the instance's BSP write.
 */
int Bricli_PrintF(BricliHandle_t *cli, const char *format, ...)
{
    char message[BRICLI_PRINT_MESSAGE_SIZE] = {0};
    int bytesWritten = 0;
    int result = -1;

    // Format the message.
    va_list args;
    va_start(args, format);
    bytesWritten = vsnprintf(message, BRICLI_PRINT_MESSAGE_SIZE, format, args);
    va_end(args);

    // Send the formatted message if it was fully written.
    if (bytesWritten > 0 && bytesWritten < BRICLI_PRINT_MESSAGE_SIZE)
    {
        result = Bricli_Write(cli, bytesWritten, message);
    }

    return result;
}