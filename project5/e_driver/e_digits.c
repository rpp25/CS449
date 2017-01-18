//rpp25. Gets the digits of e within a specified range
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "e.h"

int main(int argc, char **argv) {

	int r_start = atoi(argv[1]);	// first digit
	int r_end = atoi(argv[2]);	// last digit
	int range = r_end - r_start + 1;

	if (r_start < 0 || r_end < 0 || (r_start > r_end)) {
		printf("Invalid range");
		return 1;			// return error
	}

	FILE *file = fopen("/dev/e", "r");	// opens file for reading
	char *digits;
	if (file != NULL) {
		digits = MALLOC(range);	// allocate the required space
		fseek(file, r_start, SEEK_SET);	// seek the first digit of the request starting at the r_start th byte
		fread(digits, 1, range, file);	// Read the digits within the range
		printf("%s\n", digits);
		fclose(file);
		FREE(digits);
		return 0;
	}else{
		printf("\nInvalid or missing file.\n");
		return 1;			// return error
	}
}
