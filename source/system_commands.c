/**
 * @file system_commands.c
 * @author Anthony Wall
 * @brief Internal system command handlers
 * 
 * @copyright Copyright (c) 2025 Anthony Wall.
 *            All rights reserved
 * 
 */
#include <bricli/bricli.h>
#include "system_commands.h"
#include "auth.h"

/**
 * @brief Internal system command list
 */
const BricliSystemCommand_t SystemCommands[] =
{
    {BricliSystemCommandHelp, "help", Bricli_SystemHandlerHelp, "Displays this help message"},
    {BricliSystemCommandClear, "clear", Bricli_SystemHandlerClear, "Clears the terminal"},
    {BricliSystemCommandLogin, "login", Bricli_SystemHandlerLogin, "Login to the terminal"},
    {BricliSystemCommandLogout, "logout", Bricli_SystemHandlerLogout, "Logout from the terminal"},
    {BricliSystemCommandNone, NULL, NULL, NULL}
};

/**
 * @brief System command for displaying a help message
 *
 * @param cli Pointer to the CLI instance
 * @param numberOfArgs The number of arguments received
 * @param args The arguments list
 * @return int 0 for success, negative for error
 */
int Bricli_SystemHandlerHelp(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[])
{
    // Unused parameters
    (void)numberOfArgs;
    (void)args;

    // Print the system commands first.
    const BricliSystemCommand_t *systemCommand = &SystemCommands[0];
    while(systemCommand->ID != BricliSystemCommandNone)
    {
        // Don't display system auth commands if we aren't using auth
        if (!cli->Settings.EnableAuth && (systemCommand->ID == BricliSystemCommandLogin || systemCommand->ID == BricliSystemCommandLogout))
        {
            systemCommand++;
            continue;
        }
        
        if (cli->SendEol == NULL)
        {
            Bricli_PrintF(cli, "%s - %s%s", systemCommand->Name, systemCommand->HelpMessage, cli->Eol);
        }
        else
        {
            Bricli_PrintF(cli, "%s - %s%s", systemCommand->Name, systemCommand->HelpMessage, cli->SendEol);
        }

        // Increment the pointer
        systemCommand++;
    }

    // Print all registered user commands.
    BricliCommand_t *command = &cli->CommandList[0];
    while(command->Name != NULL)
    {
        // Only display help if the command is available to the current auth scope
        if (Bricli_IsCommandInScope(cli, command))
        {
            // Send the help message for each command.
            if (command->HelpMessage == NULL)
            {
                if (cli->SendEol == NULL)
                {
                    Bricli_PrintF(cli, "%s%s", command->Name, cli->Eol);
                }
                else
                {
                    Bricli_PrintF(cli, "%s%s", command->Name, cli->SendEol);
                }
            }
            else
            {
                if (cli->SendEol == NULL)
                {
                    Bricli_PrintF(cli, "%s - %s%s", command->Name, command->HelpMessage, cli->Eol);
                }
                else
                {
                    Bricli_PrintF(cli, "%s - %s%s", command->Name, command->HelpMessage, cli->SendEol);
                }
            }
        }

        // Increment the pointer
        command++;
    }

    return BricliOk;
}

/**
 * @brief System command for clearing the buffer
 *
 * @param cli Pointer to the CLI instance
 * @param numberOfArgs The number of arguments received
 * @param args The arguments list
 * @return int 0 for success, negative for error
 */
int Bricli_SystemHandlerClear(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[])
{
    // Unused parameters
    (void)numberOfArgs;
    (void)args;

    Bricli_ClearScreen(cli);
    return BricliOk;
}

/**
 * @brief System command for logging in as an authenticated user
 *
 * @param cli Pointer to the CLI instance
 * @param numberOfArgs The number of arguments received
 * @param args The arguments list
 * @return int 0 for success, negative for error
 */
int Bricli_SystemHandlerLogin(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[])
{
    int result = -1;

    if (numberOfArgs < 2)
    {
        Bricli_WriteStringColouredLine(cli, "ERROR: login requires 2 arguments!", BricliTextRed);
        goto cleanup;
    }

    if (NULL == cli->AuthList)
    {
        Bricli_WriteStringColouredLine(cli, "ERROR: No authentication provider registered", BricliTextRed);
        goto cleanup;
    }

    // Helper pointers for our arguments
    const char *user = args[0];
    const char *pass = args[1];

    // Search the auth provider for a matching user
    const BricliAuthEntry_t *authEntry = &cli->AuthList[0];
    while (NULL != authEntry->Username)
    {
        // Search for a matching username
        if (strcmp(user, authEntry->Username) == 0)
        {
            // Search for a matching password
            if (strcmp(pass, authEntry->Password) == 0)
            {
                // Update our active scopes
                cli->AuthScopes = authEntry->Scopes;

                // Success
                result = 0;
                if (cli->SendEol == NULL)
                {
                    Bricli_PrintF(cli, "Logged in as %s%s", authEntry->Username, cli->Eol);
                }
                else
                {
                    Bricli_PrintF(cli, "Logged in as %s%s", authEntry->Username, cli->SendEol);
                }

                goto cleanup;
            }
            else
            {
                // Password mismatch
                result = -2;
                goto cleanup;
            }
        }

        // Increment the pointer
        authEntry++;
    }

    // If we get here then we didn't find a matching user
    result = -2;

cleanup:
    // If we have an error report back
    if (result == -2)
        Bricli_WriteStringColouredLine(cli, "ERROR: Invalid username or password", BricliTextRed);

    return result;
}

/**
 * @brief System command for logging out of the current authenticated user
 *
 * @param cli Pointer to the CLI instance
 * @param numberOfArgs The number of arguments received
 * @param args The arguments list
 * @return int 0 for success, negative for error
 */
int Bricli_SystemHandlerLogout(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[])
{
    int result = 0;

    if (cli->AuthScopes != BricliScopeAll)
    {
        // Reset auth scopes
        cli->AuthScopes = BricliScopeAll;
        Bricli_WriteString(cli, "Logged out of session\n");
    }
    else
    {
        // Not logged in, so can't log out
        Bricli_WriteString(cli, "No active login session found\n");
    }

    return result;
}