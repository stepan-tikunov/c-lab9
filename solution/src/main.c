#include <stdbool.h>
#include <string.h>

#include "run_modes.h"

int main(int argc, char **argv) {
	bool is_interactive = (argc == 2) && (strcmp(argv[1], "i") == 0);

	return is_interactive ? interactive() : demo();
}
