#include <stdio.h>
#include "my_basic.h"

#include "test.h"

//note: program *should* be zero-terminated but isn't... zero-filling memory helps us out here I guess.
extern char _binary_program_bas_start;
extern char _binary_program_bas_end;


int main(int argc, char **argv) {
	struct mb_interpreter_t* bas = NULL;
	do_test();
	mb_init();
	mb_open(&bas);
	mb_load_string(bas, &_binary_program_bas_start, true);
	mb_run(bas, true);
	mb_close(&bas);
	mb_dispose();
}
