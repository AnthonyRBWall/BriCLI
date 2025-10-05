#ifndef BRICLI_SETTINGS_H
#define BRICLI_SETTINGS_H


// ===========================
// ===== System Settings =====
// ===========================

#ifndef BRICLI_ARGUMENT_BUFFER_LEN
#define BRICLI_ARGUMENT_BUFFER_LEN 70 // The maximum number of bytes available for argument storage.
#endif // BRICLI_ARGUMENT_BUFFER_LEN

#ifndef BRICLI_MAX_ARGUMENTS
#define BRICLI_MAX_ARGUMENTS 3 // Sets the maximum number of arguments that BriCLI can find.
#endif // BRICLI_MAX_ARGUMENTS

#ifndef BRICLI_MAX_COMMAND_LEN
#define BRICLI_MAX_COMMAND_LEN 10 // Sets the maximum command name length.
#endif // BRICLI_MAX_COMMAND_LEN

#ifndef BRICLI_PRINT_MESSAGE_SIZE
#define BRICLI_PRINT_MESSAGE_SIZE 80 // Sets the maximum length of a custom PrintF message.
#endif // BRICLI_PRINT_MESSAGE_SIZE

// ============================
// ===== Default Settings =====
// ============================

#ifndef BRICLI_DEFAULT_EOL
#define BRICLI_DEFAULT_EOL				"\n" // The default EoL string to be used when none is provided to init
#endif // BRICLI_DEFAULT_EOL

#ifndef BRICLI_DEFAULT_PROMPT
#define BRICLI_DEFAULT_PROMPT			">> " // The default prompt string to be used when none is provided to init
#endif // BRICLI_DEFAULT_PROMPT

// ==============================
// ===== Logging Settings =======
// ==============================

#ifndef BRICLI_LOG
#define BRICLI_LOG(...) // Logging function for displaying internal behaviour
#endif // BRICLI_LOG

/**
 * @brief Runtime configurable settings
 */
typedef struct _BricliSettings_t
{
    uint8_t EnableAuth : 1;         // Allow the use of authentication features
    uint8_t EnableColour : 1;       // Allow the use of VT100 colour options
    uint8_t EnableLocalEcho : 1;    // Echo received characters back to the caller
    uint8_t ShowHandlerErrors : 1;  // Automatically report command handler error codes
    uint8_t ShowHelpOnError : 1;    // Print the help message on receipt of an unkown command
    uint8_t __reserved : 4;         // Reserved
} BricliSettings_t;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// C functions and constants

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BRICLI_SETTINGS_H
