/**
 * @file colours.h
 * @author Anthony Wall
 * @brief VT100 colour commands
 * 
 * @copyright Copyright (c) 2025 Anthony Wall.
 *            All rights reserved
 * 
 */
#ifndef BRICLI_COLOURS_H
#define BRICLI_COLOURS_H

#include "handle.h"

// //High intensity background
// #define BRICLI_INTENSE_BKGND_BLKHB "\e[0;100m"
// #define REDHB "\e[0;101m"
// #define GRNHB "\e[0;102m"
// #define YELHB "\e[0;103m"
// #define BLUHB "\e[0;104m"
// #define MAGHB "\e[0;105m"
// #define CYNHB "\e[0;106m"
// #define WHTHB "\e[0;107m"

// //High intensty text
// #define HBLK "\e[0;90m"
// #define HRED "\e[0;91m"
// #define HGRN "\e[0;92m"
// #define HYEL "\e[0;93m"
// #define HBLU "\e[0;94m"
// #define HMAG "\e[0;95m"
// #define HCYN "\e[0;96m"
// #define HWHT "\e[0;97m"

// //Bold high intensity text
// #define BHBLK "\e[1;90m"
// #define BHRED "\e[1;91m"
// #define BHGRN "\e[1;92m"
// #define BHYEL "\e[1;93m"
// #define BHBLU "\e[1;94m"
// #define BHMAG "\e[1;95m"
// #define BHCYN "\e[1;96m"
// #define BHWHT "\e[1;97m"

/**
 * @brief Enumerated VT100 colour options
 */
typedef enum _BricliColours_t
{
    BricliColourReset,

    BricliTextBlack,
    BricliTextRed,
    BricliTextGreen,
    BricliTextYellow,
    BricliTextBlue,
    BricliTextMagenta,
    BricliTextCyan,
    BricliTextWhite,

    BricliTextBoldBlack,
    BricliTextBoldRed,
    BricliTextBoldGreen,
    BricliTextBoldYellow,
    BricliTextBoldBlue,
    BricliTextBoldMagenta,
    BricliTextBoldCyan,
    BricliTextBoldWhite,

    BricliUnderlineBlack,
    BricliUnderlineRed,
    BricliUnderlineGreen,
    BricliUnderlineYellow,
    BricliUnderlineBlue,
    BricliUnderlineMagenta,
    BricliUnderlineCyan,
    BricliUnderlineWhite,

    BricliBackgroundBlack,
    BricliBackgroundRed,
    BricliBackgroundGreen,
    BricliBackgroundYellow,
    BricliBackgroundBlue,
    BricliBackgroundMagenta,
    BricliBackgroundCyan,
    BricliBackgroundWhite,

    BricliColourCount
} BricliColours_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern void Bricli_SetColour(BricliHandle_t* cli, BricliColours_t colourId);


/**
 * @brief Helper macro for calling Bricli_PrintF with colour support.
 *
 * @param cli Pointer to the CLI instance to use.
 * @param colour The colour to be used for data.
 * @param format Format string to be used for message generation.
 * @param va_args An additional variadic argument list to be used with Bricli_PrintF
 *
 * @return The error code from the instance's BSP write.
 */
#define BRICLI_PRINTF_COLOURED(cli, colour, format, ...) ({ Bricli_SetColour(cli, colour); int macroResult = Bricli_PrintF(cli, format, __VA_ARGS__); Bricli_SetColour(cli, BricliColourReset); macroResult; })

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BRICLI_COLOURS_H
