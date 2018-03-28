#include <stdio.h>

int number_of_process;
int main (int argc, char** argv) {
	// if more or less than one argument, prints error
	if(argc != 2) {
		printf("Error");
	}

	//converts number of process to integer
	else {
		number_of_process = atoi(argv[1])
	}

	int allocation[number_of_process];
	memset(allocation, -1, sizeof(allocation));
	return 0;
}