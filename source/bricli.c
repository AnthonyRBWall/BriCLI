/**
 * @file    bricli.c
 * @brief   BriCLI source, provides a lightweight CLI implementation.
 *          Supports VT100 ANSI escape codes.
 * @author  Anthony Wall
 *
 * Copyright (C) 2025 Anthony Wall.
 * All rights reserved.
 *
 **/

// ====================
// ===== Includes =====
// ====================

// Fix for "strtok_r" not found on default C99 GNU builds.
#ifdef __GNUC__
    #define _GNU_SOURCE
#endif // __GNUC__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <bricli/bricli.h>
#include "system_commands.h"
#include "auth.h"

// =====================
// ===== Constants =====
// =====================

// ===========================
// ===== Local Functions =====
// ===========================

/**
 * @brief Extracts arguments from a given argument string. Arguments must be separated by spaces.
 *
 * @param arguments     Pointer to the argument string to look for arguments in.
 * @param output        Pointer to a char* array to store each of the found arguments in.
 *
 * @return The number of arguments found.
 */
static uint32_t Bricli_ExtractArguments(char *arguments, char *output[])
{
    // char seperator[] = " ";
    uint8_t argumentsFound = 0;
    // char *savePointer = NULL;
    char *argumentsEnd = NULL;

    // printf("[DEBUG] %s: %s\n", __func__, arguments);

    // Make sure we actually have something to work with.
    if (arguments == NULL || output == NULL || *arguments == '\0')
    {
        return 0;
    }

    // Find the end of the string
    argumentsEnd = arguments + strlen(arguments);
    // printf("[DEBUG] Args Tokenizer: %p to %p (strlen: %u)\n", arguments, argumentsEnd, (uint32_t)strlen(arguments));

    // Setup tokenizer variables
    char *argStart = arguments;
    char *cursor = argStart;
    bool isStringMode = false;
    bool argFound = false;

    // Run the tokenizer
    while (argStart != argumentsEnd)
    {
        char *argEnd = strchr(cursor, ' ');
        bool moreArgs = (argEnd != NULL);

        // Normal parsing mode
        if (!isStringMode)
        {
            // String found, switch mode
            if (*cursor == '\"')
            {
                // printf("[DEBUG] Entering string mode\n");
                isStringMode = true;

                // Strip out the starting quote
                strcpy(cursor, (cursor + 1));
                argumentsEnd--;
            }
            else
            {
                // Non-string arguments require no additional-processing
                argFound = true;
            }
        }
        // String parsing mode
        else
        {
            // Search for the end sting
            cursor = strchr(cursor, '\"');

            // Missing closing quote mark, exit
            if (NULL == cursor)
            {
                // printf("[DEBUG] Invalid string format\n");
                return 0;
            }
            else if (*(cursor - 1) == '\\')
            {
                // Escaped quote mark, ignore
                // printf("[DEBUG] Skipping escape character\n");
                cursor++;
            }
            else
            {
                // Closing quote mark, string found
                // printf("[DEBUG] Exiting string mode\n");

                // Strip the quote mark out
                strcpy(cursor, (cursor + 1));
                argumentsEnd--;

                // Move argEnd pointer and update tokenizer variables
                argEnd = cursor;
                argFound = true;
                isStringMode = false;
            }
        }

        // An argument was found
        if (argFound)
        {
            // Store the argument
            output[argumentsFound] = argStart;
            argumentsFound++;

            // Walk through the argument and replace any escapes
            cursor = strchr(argStart, '\\');
            while (NULL != cursor)
            {
                // printf("[DEBUG] Searching for '\\'\n");
                cursor = strchr(cursor, '\\');
                if (NULL != cursor && *cursor == '\\')
                {
                    // printf("[DEBUG] Replaced escaped character with %c\n", *(cursor+1));
                    strcpy(cursor, (cursor + 1));
                    cursor++;
                    argumentsEnd--;

                    if (NULL != argEnd)
                    {
                        // printf("[DEBUG] Moving argEnd\n");
                        argEnd--;
                    }
                }
            }


            // Process additional arguments if required
            if (!moreArgs)
            {
                // No more arguments
                // printf("[DEBUG] Tokenizer finished\n");
                argEnd = argumentsEnd;
            }
            else
            {
                // More arguments
                // printf("[DEBUG] More arguments: %020x\n", *argEnd);
                *argEnd = '\0';
                argEnd++;
            }

            // printf("[DEBUG] Arg%d: %s\n", argumentsFound, argStart);

            // Reset tokenizer
            argFound = false;
            argStart = argEnd;
            cursor = argStart;

        }
    }

    // Return how many arguments we were able to find.
    return argumentsFound;
}

/**
 * @brief Update the state of a given BriCLI handle, calling the event handler if set.
 *
 * @param cli Pointer to the BriCLI instance to use.
 * @param newState The new state to set on the BriCLI instance.
 */
static inline void Bricli_ChangeState(BricliHandle_t* cli, BricliStates_t newState)
{
    BricliStates_t currentState = cli->State;

    // Update our state and call the OnStateChanged event if set.
    cli->State = newState;
    if (cli->OnStateChanged != NULL)
    {
        cli->OnStateChanged(currentState, newState);
    }
}

// ==============================
// ===== Exported Functions =====
// ==============================

/**
 * @brief Initialises a BriCLI instance using the given settings list
 * 
 * @param cli Pointer to a BriCLI instance to initialise
 * @param init Pointer to a settings list to use for instance customization
 * @return BricliErrors_t 
 */
BricliErrors_t Bricli_Init(BricliHandle_t *cli, const BricliInit_t* init)
{
    BricliErrors_t result = BricliUnknown;

    // Validate pointers
    if (NULL == cli || NULL == init)
    {
        BRICLI_LOG("BriCLI: NULL pointer in %s\n", __func__);
        result = BricliBadParameter;
        goto cleanup;
    }

    // Validate buffer settings
    if (NULL == init->RxBuffer || 0 == init->RxBufferSize)
    {
        BRICLI_LOG("BriCLI: Invalid RX buffer in %s\n", __func__);
        result = BricliBadParameter;
        goto cleanup;
    }

    // Validate command list settings
    if (NULL == init->CommandList || NULL == init->CommandList[0].Name)
    {
        BRICLI_LOG("BriCLI: Invalid Command List in %s\n", __func__);
        result = BricliBadParameter;
        goto cleanup;
    }

    // Validate BSP Write function
    if (NULL == init->BspWrite)
    {
        BRICLI_LOG("BriCLI: Invalid BspWrite in %s\n", __func__);
        result = BricliBadParameter;
        goto cleanup;
    }

    // Initialise the CLI to zero
    memset(cli, 0, sizeof(BricliHandle_t));

    // EOL string
    if (NULL != init->Eol)
        cli->Eol = init->Eol;
    else
        cli->Eol = BRICLI_DEFAULT_EOL;

    // Prompt string
    if (NULL != init->Prompt)
        cli->Prompt = init->Prompt;
    else
        cli->Prompt = BRICLI_DEFAULT_PROMPT;

    // RX Buffer settings
    cli->RxBuffer = init->RxBuffer;
    cli->RxBufferSize = init->RxBufferSize;

    // Command List settings
    cli->CommandList = init->CommandList;

    // BSP settings
    cli->BspWrite = init->BspWrite;

    // Event settings
    cli->OnStateChanged = init->OnStateChanged;

    // Runtime settings
    memcpy(&cli->Settings, &init->Settings, sizeof(BricliSettings_t));

    // Auth list
    if (NULL != init->AuthList)
        cli->AuthList = init->AuthList;

    // Success
    BRICLI_LOG("BriCLI: Initialised\n");
    result = BricliOk;

cleanup:
    return result;
}

int Bricli_ParseEscapeCode(BricliHandle_t *cli)
{
    //  // Check if this is an up arrow or not.
    //  if (cli->PendingBytes >= BRICLI_ARROW_LEN && (strncmp(cli->RxBuffer, BRICLI_UP_ARROW, cli->BRICLI_ARROW_LEN) ==0) )
    //    {
    //      // Up Arrow pressed.
    //    }
    (void)&cli;
    return BricliOk;
}

/**
 * @brief Removes a command from the receive buffer, moving any remaining commands to the front.
 *
 * @param cli Pointer to a BriCLI instance.
 */
void Bricli_ClearCommand(BricliHandle_t *cli)
{
    size_t nextCommand = 0;

    // If there is another command it will always be EOL length past our old command.
    nextCommand = strlen(cli->RxBuffer) + strlen(cli->Eol);

    // If the next command is out of bounds or more than we have just clear the whole buffer.
    if (nextCommand >= cli->RxBufferSize || nextCommand >= cli->PendingBytes)
    {
        Bricli_ClearBuffer(cli);
    }
    else
    {
        // Remove the number of bytes we have handled.
        cli->PendingBytes -= nextCommand;

        // Shift next command to start of buffer
        memmove(cli->RxBuffer, &cli->RxBuffer[nextCommand], cli->PendingBytes);
        memset(&cli->RxBuffer[cli->PendingBytes], 0, nextCommand);
    }
}

/**
* @brief Default runner for performing common BriCLI functionality.
*
* @param cli Pointer to the CLI instance to use.
* @return The error code from Bricli_ParseCommand.
*/
int Bricli_Parse(BricliHandle_t *cli)
{
    size_t numberOfCommands;
    int result = BricliOk;

    // First do a non-invasive check for an EOL delimeter.
    if (!Bricli_CheckForEol(cli, false))
    {
        goto cleanup;
    }

    // Edge case: Eol has been sent on it's own
    // giving us a zero-length command.
    if (cli->PendingBytes == strlen(cli->Eol))
    {
        BRICLI_LOG("BriCLI: 0 length command received, skipping\n");
        Bricli_SendPrompt(cli);
        Bricli_ClearBuffer(cli);
        goto cleanup;
    }

    // Look for an EOL, repeating for as long as we have commands in the buffer.
    numberOfCommands = Bricli_SplitOnEol(cli);
    while(numberOfCommands > 0)
    {
        // Handle the command.
        result = Bricli_ParseCommand(cli);

        // Remove the command we just handled
        Bricli_ClearCommand(cli);

        // Reset our internal state.
        Bricli_ChangeState(cli, BricliStateIdle);

        // Track that we have handled this command.
        numberOfCommands--;

        // If we just handled the last command send the CLI prompt.
        if (numberOfCommands == 0)
        {
            Bricli_SendPrompt(cli);
        }
    }

cleanup:
    return result;
}


/**
 * @brief Parses the given command against the provided CLI instance.
 *
 * @param cli Pointer to a BriCLI instance.
 * @param data The full command string received including any arguments.
 *
 * @return Pass through return from the given command handler.
 */
int Bricli_ParseCommand(BricliHandle_t *cli)
{
    char command[BRICLI_MAX_COMMAND_LEN + 1] = {0};
    char arguments[BRICLI_ARGUMENT_BUFFER_LEN] = {0};
    char *ArgumentsFound[BRICLI_MAX_ARGUMENTS] = {0};
    uint32_t commandLength = 0;
    uint32_t argumentLength = 0;

    // Error check our arguments.
    if (cli->RxBuffer == NULL)
    {
        cli->LastError = BricliErrorInternal;
        return BricliBadParameter;
    }
    else if (cli == NULL || cli->CommandList == NULL)
    {
        cli->LastError = BricliErrorInternal;
        return BricliBadHandle;
    }

    // Update our state.
    Bricli_ChangeState(cli, BricliStateParsing);

    // If this is actually an escape sequence handle it separately.
    if (cli->RxBuffer[0] == '\e')
    {
        return Bricli_ParseEscapeCode(cli);
    }

    // Look for arguments.
    char *argData = strchr(cli->RxBuffer, ' ');

    // Split the command and arguments if needed.
    if (argData != NULL)
    {
        // Calculate length of command and skip the first space in argData.
        commandLength = argData - cli->RxBuffer;
        argData++;

        // Copy the arguments into our local buffer, ensuring we don't overflow.
        argumentLength = strlen(argData);
        if (argumentLength > BRICLI_ARGUMENT_BUFFER_LEN)
        {
            argumentLength = BRICLI_ARGUMENT_BUFFER_LEN;
        }
        memcpy(arguments, argData, argumentLength);
    }
    else
    {
        commandLength = strlen(cli->RxBuffer);
    }

    // Limit the command length to prevent overflow.
    if (commandLength > BRICLI_MAX_COMMAND_LEN)
    {
        commandLength = BRICLI_MAX_COMMAND_LEN;
    }
    memcpy(command, (void *)cli->RxBuffer, commandLength);

    // Extract additional arguments.
    uint8_t numberOfArguments = Bricli_ExtractArguments(arguments, ArgumentsFound);

    // Check if this is a system command.
    const BricliSystemCommand_t *systemCommand = &SystemCommands[0];
    while (systemCommand->ID != BricliSystemCommandNone)
    {
        // Don't display system auth commands if we aren't using auth
        if (!cli->Settings.EnableAuth && (systemCommand->ID == BricliSystemCommandLogin || systemCommand->ID == BricliSystemCommandLogout))
        {
            systemCommand++;
            continue;
        }
        
        if (strcmp(command, systemCommand->Name) == 0)
        {
            // Call the command's handler function.
            Bricli_ChangeState(cli, BricliStateHandlerRunning);
            systemCommand->Handler(cli, numberOfArguments, ArgumentsFound);
            Bricli_ChangeState(cli, BricliStateFinished);

            return BricliOk;
        }

        // Increment the pointer
        systemCommand++;
    }

    // Not a system command so look to our command list for a match.
    BricliCommand_t *cliCommand = &cli->CommandList[0];
    while(cliCommand->Name != NULL)
    {
        // Check if we have found a match.
        if (strcmp(command, cliCommand->Name) == 0)
        {
            int result = BricliUnknown;

            // Check our authentication scopes
            if (Bricli_IsCommandInScope(cli, cliCommand))
            {
                // Call the command's handler function.
                Bricli_ChangeState(cli, BricliStateHandlerRunning);
                result = cliCommand->Handler(numberOfArguments, ArgumentsFound);
                Bricli_ChangeState(cli, BricliStateFinished);

                // Check the result code.
                if (result < 0)
                {
                    // If enabled, display the error code to the user.
                    if (cli->Settings.ShowHandlerErrors)
                    {
                        if (cli->SendEol == NULL)
                        {
                            BRICLI_PRINTF_COLOURED(cli, BricliTextRed, "Command returned error: %d%s", result, cli->Eol);
                        }
                        else
                        {
                            BRICLI_PRINTF_COLOURED(cli, BricliTextRed, "Command returned error: %d%s", result, cli->SendEol);
                        }
                    }

                    cli->LastError = BricliErrorCommand;
                }
            }
            else
            {
                BRICLI_LOG("BriCLI: Attempted to call authorized command \"%s\" with invalid auth scope\n", command);
                cli->LastError = BricliErrorInternal;
                result = BricliUnauthorized;

                if (cli->SendEol == NULL)
                {
                    Bricli_PrintF(cli, "Unknown Command %s%s", command, cli->Eol);
                }
                else
                {
                    Bricli_PrintF(cli, "Unknown Command %s%s", command, cli->SendEol);
                }
            }

            return result;
        }

        // Increment the pointer
        cliCommand++;
    }

    // If we get here then we failed to find a valid command in the list.
    if (cli->SendEol == NULL)
    {
        Bricli_PrintF(cli, "Unknown Command %s%s", command, cli->Eol);
    }
    else
    {
        Bricli_PrintF(cli, "Unknown Command %s%s", command, cli->SendEol);
    }

    // If enabled, print help on an unknown command.
    if (cli->Settings.ShowHelpOnError)
    {
        Bricli_PrintHelp(cli);
    }

    // Return that this is an unknown command.
    cli->LastError = BricliErrorInternal;
    return BricliBadCommand;
}

/**
* @brief Checks a given buffer for an occurrence of the EoL string.
*
* @param cli        Pointer to the CLI instance to use.
* @param data       Pointer to the buffer to search.
* @param replaceEol True to replace the EOL with '\0', false to leave it alone.
*
* @return True when an EOL is found, false otherwise.
*/
bool Bricli_CheckForEol(BricliHandle_t *cli, bool replaceEol)
{
    char *eol = NULL;
    bool result = false;

    // Make sure our parameters are valid.
    if (cli == NULL || cli->Eol == NULL || cli->RxBuffer == NULL || cli->PendingBytes == 0)
    {
        goto cleanup;
    }

    // Look for the EOL substring in our data.
    eol = strstr(cli->RxBuffer, cli->Eol);
    if (eol == NULL)
    {
        // No EOL found.
        result = false;
    }
    else
    {
        // EOL found.
        result = true;

        // Inject null characters if needed.
        if (replaceEol)
        {
            memset(eol, '\0', strlen(cli->Eol));
        }
    }

cleanup:
    return result;
}

/**
* @brief For the given buffer, searches for all EOL strings and replaces them with null characters.
*
* @param cli Pointer to the CLI instance to use.
*
* @return The number of commands found in the buffer.
*/
size_t Bricli_SplitOnEol(BricliHandle_t *cli)
{
    uint16_t numberOfCommands = 0;
    char *token = NULL;
    char *lastToken = NULL;

    // Make sure our parameters are valid.
    if (cli == NULL || cli->Eol == NULL || cli->RxBuffer == NULL || cli->PendingBytes == 0)
    {
        goto cleanup;
    }

    // Iterate over the buffer looking for EOLs.
    token = (char *)strtok_r(cli->RxBuffer, cli->Eol, &lastToken);

    while (token != NULL)
    {
        // Increment the number of commands and get the next token.
        numberOfCommands++;
        token = (char *)strtok_r(NULL, cli->Eol, &lastToken);
    }

    // Edge case: If no delimiters were present the length of the buffer will match pending bytes exactly.
    // In this case an EOL was not found and the numberOfCommands is actually off by one.
    if (numberOfCommands == 1 && strlen(cli->RxBuffer) == cli->PendingBytes)
    {
        numberOfCommands--;
    }


cleanup:
    // Return how many commands we found.
    return numberOfCommands;

}

/**
 * @brief Places a character into the RX buffer and performs various character checks.
 *
 * @param cli Pointer to the CLI instance to use.
 * @param rxChar The character received.
 *
 * @return BricliCopyWouldOverflow if the RX buffer is full, BricliOk otherwise.
 */
BricliErrors_t Bricli_ReceiveCharacter(BricliHandle_t *cli, char rxChar)
{
    BricliErrors_t result = BricliUnknown;

    // Check for a null terminating character.
    if (rxChar == '\0')
    {
        result = BricliReceivedNull;
        goto cleanup;
    }

    // Check for an overflow.
    if (cli->PendingBytes >= cli->RxBufferSize)
    {
        result = BricliCopyWouldOverflow;
        goto cleanup;
    }

    // Store the received character.
    cli->RxBuffer[cli->PendingBytes] = rxChar;
    cli->PendingBytes++;

    // If we are currently in the process of handling an escape code.
    if (cli->IsHandlingEscape)
    {
        Bricli_ParseEscapeCode(cli);
    }

    // If we've received a backspace then we need to remove it and the previous character.
    if (rxChar == '\b')
    {
        Bricli_Backspace(cli);
        result = BricliOk;
    }

    // Echo the received character.
    if (cli->Settings.EnableLocalEcho)
    {
        Bricli_Write(cli, 1, &rxChar);
    }

    // Success.
    result = BricliOk;

cleanup:
    return result;
}

/**
 * @brief Iterates through an array of received characters, parsing each in turn.
 *
 * @param cli       Pointer to the CLI instance to use.
 * @param index     The starting character in array to receive from.
 * @param length    The number of bytes to be received from the array.
 * @param arrray    Pointer to the array to be received.
 *
 * @return BricliCopyWouldOverflow if the RX buffer is full, BricliOk otherwise.
 */
BricliErrors_t Bricli_ReceiveIndexedArray(BricliHandle_t *cli, uint32_t index, uint32_t length, char *array)
{
    BricliErrors_t error = BricliUnknown;

    // Receive each character in turn.
    for (uint32_t i = index; i < (index + length); i++)
    {
        error = Bricli_ReceiveCharacter(cli, array[i]);

        // If something went wrong, exit immediately.
        if (error != BricliOk)
        {
            return error;
        }
    }
    return error;
}

/** @brief Helper function that clears the internal buffer and resets the CLI state.
 *
 * @param cli Pointer to a BriCLI instance.
 */
void Bricli_Reset(BricliHandle_t *cli)
{
    Bricli_ClearBuffer(cli);
    Bricli_ChangeState(cli, BricliStateIdle);
}
