/**
 * @file    bricli.h
 * @brief   Header for the BriCLI library.
 * @author  Anthony Wall
 *
 * Copyright (C) 2025 Anthony Wall.
 * All rights reserved.
 *
 **/

#ifndef BRICLI_CONFIG_H
#define BRICLI_CONFIG_H

// ===========================
// ===== System Settings =====
// ===========================

// The length of the internal arguments buffer, default 70
#define BRICLI_ARGUMENT_BUFFER_LEN 70

// The maximum number of arguments BriCLI can parse, default 3
#define BRICLI_MAX_ARGUMENTS 3

// The maximum length any user command can be, default 10
#define BRICLI_MAX_COMMAND_LEN 10

// The maximum length a PrintF message can be, default 80
#define BRICLI_PRINT_MESSAGE_SIZE 80

// ============================
// ===== Default Settings =====
// ============================

// The default EoL string to be used when none is provided to init
#define BRICLI_DEFAULT_EOL				"\n"

// The default prompt string to be used when non is provided to init
#define BRICLI_DEFAULT_PROMPT			">> "

// ==============================
// ===== Logging Settings =======
// ==============================

/**
 * @brief Logging function for displaying internal behaviour
 */
#define BRICLI_LOG(...) printf(__VA_ARGS__)

#endif // BRICLI_CONFIG_H
