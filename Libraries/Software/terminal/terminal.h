#pragma once

#include <stdint.h>

#define TM_MAX_TOTAL_ARG_LEN ((uint8_t)32)
#define TM_MAX_CMD_NUM ((uint8_t)16)

#if 0
extern int TM_printf(char const* command, ...); // Terminal output
#else
extern int printf_(char const* command, ...); // Terminal output
#define TM_printf printf_
#endif

void TM_onChar(char); // Terminal input

// argv[0] command name
typedef void (*TM_callback)(char const* argv[], uint8_t argn);

void TM_registerCommand(char const* command, TM_callback cb);

