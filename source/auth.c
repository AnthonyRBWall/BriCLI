/**
 * @file auth.c
 * @author Anthony Wall
 * @brief Authentication features
 * 
 * @copyright Copyright (c) 2025 Anthony Wall.
 *            All rights reserved
 * 
 */
#include <stdbool.h>
#include "auth.h"

/**
 * @brief Validates whether the given command is available under the current auth scopes
 *
 * @param cli Pointer to the BriCLI instance to use
 * @param command Pointer to a command to verify
 * @return bool True if the command is within auth scope, false otherwise
 */
bool Bricli_IsCommandInScope(BricliHandle_t* cli, BricliCommand_t *command)
{
    if (command->AuthScopesRequired == BricliScopeAll)
    {
        // Command is available to all scopes
        return true;
    }
    else if (cli->Settings.EnableAuth && ((cli->AuthScopes & command->AuthScopesRequired) != 0) )
    {
        // Command is available under granted scopes
        return true;
    }
    else
    {
        // Command is not available
        return false;
    }
}