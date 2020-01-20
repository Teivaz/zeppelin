#include "terminal.h"
#include <string.h>

typedef struct Command {
	char const* name;
	TM_callback func;
} Command;

enum Keys {
	Key_Enter = 0x0d,
	Key_Del = 0x7f,
	Key_Esc = 0x1b,
};

static Command s_commands[TM_MAX_CMD_NUM];
static uint8_t s_commandNum = 0;

static char s_buffer[TM_MAX_TOTAL_ARG_LEN] = {0};
static char* s_cmd = s_buffer;


static uint8_t isPrintable(char c) {
	return (c >= 0x20 && c < 0x7f);
}
static uint8_t isCtrl(char c) {
	return (c == Key_Enter || c == Key_Del || c == Key_Esc);
}
static uint8_t isValid(char c) {
	return isPrintable(c) || isCtrl(c);
}

static uint8_t replaceFirst(char* data, char const* end) {
	if (data >= end) return 0;
	uint8_t pos = 0;
	while (data < end) {
		if (*data == 0) {
			return pos;
		}
		if (*data == ' ') {
			*data = 0;
			return pos;
		}
		++data;
		++pos;
	}
	return 0;
}

static void processCmd() {
	char const* argv[TM_MAX_TOTAL_ARG_LEN/2 + 1];
	char const* const end = s_buffer + strlen(s_buffer) + 1;
	char* ptr = s_buffer;

	uint8_t argn = 0;
	uint8_t offset = 0;

	while ((offset = replaceFirst(ptr, end))) {
		argv[argn] = ptr;
		ptr += offset + 1;
		++argn;
	}
	if (argn == 0) {
		return;
	}
	Command* cmd = 0;
	for (uint8_t i = 0; i <= s_commandNum; ++i) {
		if (strcmp(argv[0], s_commands[i].name) == 0) {
			cmd = &s_commands[i];
			break;
		}
	}
	if (cmd == 0) {
		TM_printf("No such command: '%s'\r\n", argv[0]);
	}
	else {
		cmd->func(argv, argn);
	}
}

void TM_onChar(char c) {
	if (!isValid(c)) {
		return;
	}
	if (c == Key_Esc) {
		s_cmd = s_buffer;
		*s_cmd = 0;
		TM_printf("\r                            \r");
	}
	else if (c == Key_Del) {
		--s_cmd;
		if (s_cmd < s_buffer) {
			s_cmd = s_buffer;
		}
		*s_cmd = ' ';
		TM_printf("\r%s", s_buffer);
		*s_cmd = 0;
		// Reprint to move cursor to the actual position
		TM_printf("\r%s", s_buffer);
	}
	else if (c == Key_Enter) {
		TM_printf("\r\n");
		processCmd();
		s_cmd = s_buffer;
		*s_cmd = 0;
	}
	
	if (s_cmd == s_buffer + TM_MAX_TOTAL_ARG_LEN - 1) {
		return;
	}
	if (isPrintable(c)) {
		*s_cmd++ = c;
		*s_cmd = 0;
		TM_printf("%c", c);
	}
}

void TM_registerCommand(char const* name, TM_callback func) {
	if (s_commandNum > TM_MAX_CMD_NUM) {
		TM_printf("Failed adding command '%s'. Exceeded maxinal number of commands (%d). Consider increasing TM_MAX_CMD_NUM.\r\n", name, TM_MAX_CMD_NUM);
		return;
	}
	Command* cmd = &s_commands[s_commandNum++];
	cmd->name = name;
	cmd->func = func;
}
