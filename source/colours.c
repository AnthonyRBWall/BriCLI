/**
 * @file colours.c
 * @author Anthony Wall
 * @brief VT100 colour functionality
 * 
 * @copyright Copyright (c) 2025 Anthony Wall.
 *            All rights reserved
 * 
 */

#include <bricli/bricli.h>

 // Contains VT100 colour codes
static const char *_colourTable[BricliColourCount] =
{
    "\e[0m",    // Control - Reset

    "\e[0;30m", // Text - Black
    "\e[0;31m", // Text - Red
    "\e[0;32m", // Text - Green
    "\e[0;33m", // Text - Yellow
    "\e[0;34m", // Text - Blue
    "\e[0;35m", // Text - Magenta
    "\e[0;36m", // Text - Cyan
    "\e[0;37m", // Text - White

    "\e[1;30m", // Bold - Black
    "\e[1;31m", // Bold - Red
    "\e[1;32m", // Bold - Green
    "\e[1;33m", // Bold - Yellow
    "\e[1;34m", // Bold - Blue
    "\e[1;35m", // Bold - Magenta
    "\e[1;36m", // Bold - Cyan
    "\e[1;37m", // Bold - White

    "\e[4;30m", // Underline - Black
    "\e[4;31m", // Underline - Red
    "\e[4;32m", // Underline - Green
    "\e[4;33m", // Underline - Yellow
    "\e[4;34m", // Underline - Blue
    "\e[4;35m", // Underline - Magenta
    "\e[4;36m", // Underline - Cyan
    "\e[4;37m", // Underline - White

    "\e[40m", // Background - Black
    "\e[41m", // Background - Red
    "\e[42m", // Background - Green
    "\e[43m", // Background - Yellow
    "\e[44m", // Background - Blue
    "\e[45m", // Background - Magenta
    "\e[46m", // Background - Cyan
    "\e[47m"  // Background - White
};

 /**
 * @brief Sets the various colour options of a VT100 terminal.
 *
 * @param colourId The enum ID of the colour option to be written.
 */
void Bricli_SetColour(BricliHandle_t *cli, BricliColours_t colourId)
{
    // If enabled, Send the colour string
    if (NULL != cli && cli->Settings.EnableColour)
    {
        Bricli_WriteString(cli, (char *)_colourTable[colourId]);
    }
}