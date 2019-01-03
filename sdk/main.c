#include <stdio.h>
#include "my_basic.h"
#include "loader.h"
#include "test.h"

int main(int argc, char **argv) {
	uint8_t *basfile=load_file("program.bas");
	struct mb_interpreter_t* bas = NULL;
	mb_init();
	mb_open(&bas);
	mb_load_string(bas, basfile, true);
	mb_run(bas, true);
	mb_close(&bas);
	mb_dispose();
	do_test();
}
