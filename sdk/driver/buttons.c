#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "buttons.h"
#include "gloss/mach_defines.h"

extern volatile uint32_t BUTTON[];
#define BUTTONREG(i) BUTTON[i/4]

int buttons_get_state() {
	return BUTTONREG(BUTTON_READ_REG);
}
