#ifndef BRICLI_SYSTEM_COMMANDS_H
#define BRICLI_SYSTEM_COMMANDS_H

#include <bricli/bricli.h>

/**
 * @brief Command handler function, one should be provided for every command.
 *
 * @param numberOfArgs The number of arguments found.
 * @param args         Array of argument string found.
 */
typedef int (*Bricli_SystemCommandHandler)(BricliHandle_t *cli, uint32_t numberOfArgs, char* args[]);

/**
 * @brief Internal IDs for tracking system commands
 */
typedef enum _BricliSystemCommandId_t
{
    BricliSystemCommandNone,
    BricliSystemCommandHelp,
    BricliSystemCommandClear,
    BricliSystemCommandLogin,
    BricliSystemCommandLogout
} BricliSystemCommandId_t;

/**
 * @brief Internal command handler
 */
typedef struct _BricliSystemCommand_t
{
    BricliSystemCommandId_t         ID;             /*<< Command ID */
    const char*                     Name;           /*<< Command name. */
    Bricli_SystemCommandHandler     Handler;        /*<< Handler function for this command. */
    const char*                     HelpMessage;    /*<< Optional message to be displayed by the help command. */
} BricliSystemCommand_t;

extern const BricliSystemCommand_t SystemCommands[];

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int Bricli_SystemHandlerHelp(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[]);
extern int Bricli_SystemHandlerClear(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[]);
extern int Bricli_SystemHandlerLogin(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[]);
extern int Bricli_SystemHandlerLogout(BricliHandle_t *cli, uint32_t numberOfArgs, char *args[]);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BRICLI_SYSTEM_COMMANDS_H
