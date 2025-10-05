#ifndef BRICLI_SETTINGS_H
#define BRICLI_SETTINGS_H

#ifndef BRICLI_MAX_COMMAND_LEN
#define BRICLI_MAX_COMMAND_LEN 10 // Sets the maximum command name length.
#endif // BRICLI_MAX_COMMAND_LEN

#ifndef BRICLI_ARGUMENT_BUFFER_LEN
#define BRICLI_ARGUMENT_BUFFER_LEN 70 // The maximum number of bytes available for argument storage.
#endif // BRICLI_ARGUMENT_BUFFER_LEN

#ifndef BRICLI_MAX_ARGUMENTS
#define BRICLI_MAX_ARGUMENTS 3 // Sets the maximum number of arguments that BriCLI can find.
#endif // BRICLI_MAX_ARGUMENTS

//#ifndef BRICLI_RX_BUFFER_LEN
//#define BRICLI_RX_BUFFER_LEN 80 // Sets the character
//#endif // BRICLI_RX_BUFFER_LEN

#ifndef BRICLI_PRINT_MESSAGE_SIZE
#define BRICLI_PRINT_MESSAGE_SIZE 80 // Sets the maximum length of a custom PrintF message.
#endif // BRICLI_PRINT_MESSAGE_SIZE

// VT100 colour options
#if 0
#warning "Colour flags are deprecated for removal in a future release"
#ifndef BRICLI_USE_BOLD
#define BRICLI_USE_BOLD            1 // Enables the use of bold text options.
#endif // BRICLI_USE_BOLD

#ifndef BRICLI_USE_UNDERLINE
#define BRICLI_USE_UNDERLINE       1 // Enables the use of underline text options.
#endif // BRICLI_USE_UNDERLINE

#ifndef BRICLI_USE_BACKGROUNDS
#define BRICLI_USE_BACKGROUNDS     1 // Enables the use of background colour options.
#endif // BRICLI_USE_BACKGROUNDS

#ifndef BRICLI_USE_INTENSE_BACKGROUNDS
// #define BRICLI_USE_INTENSE_BACKGROUNDS 1 // Enables the use of high intensity background colour options.
#endif // BRICLI_USE_INTENSE_BACKGROUNDS

#ifndef BRICLI_USE_INTENSE_TEXT
// #define BRICLI_USE_INTENSE_TEXT     1 // Enables the use of high intensity text colour options.
#endif // BRICLI_USE_INTENSE_TEXT

#ifndef BRICLI_USE_INTENSE_BOLD
// #define BRICLI_USE_INTENSE_BOLD     1 // Enables the use of high intensity bold text options.
#endif // BRICLI_USE_INTENSE_BOLD

#ifndef BRICLI_DEFAULT_EOL
#define BRICLI_DEFAULT_EOL				"\n" // The default EoL string to be used when none is provided to init
#endif // BRICLI_DEFAULT_EOL

#ifndef BRICLI_DEFAULT_PROMPT
#define BRICLI_DEFAULT_PROMPT			">> " // The default prompt string to be used when non is provided to init
#endif // BRICLI_DEFAULT_PROMPT
#endif // 0

/**
 * @brief Runtime configurable settings
 */
typedef struct _BricliSettings_t
{
    uint8_t EnableColour : 1;       // Allow the use of VT100 colour options.
    uint8_t EnableLocalEcho : 1;    // Echo received characters back to the caller
    uint8_t ShowHandlerErrors : 1;  // Automatically report command handler error codes.
    uint8_t ShowHelpOnError : 1;    // Print the help message on receipt of an unkown command.
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
