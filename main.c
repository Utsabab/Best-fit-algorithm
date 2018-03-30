#include <stdio.h>

int number_of_process;
int memsize[4096] = {0};
int num_holes;
int* getholes()

int main (int argc, char** argv) {
	// if more or less than one argument, prints error
	if(argc != 2) {
		printf("Error");
	}

	//converts number of process to integer
	else {
		number_of_process = atoi(argv[1])
	}

	return 0;
}


int* getholes() {
	int count = 0;
	int i;
	int initial;
	int final;
	for (i=0;i<4095;i++) {
		if (memsize[i] == 0 && memsize[i + 1] == 1) {
			count = count + 1;
		}
	} 
	if (memsize[4095] == 0) {
		count = count + 1;
	}

	num_holes = count;

	int new_block = 0;
	int* holes = (int *) malloc ((count * 2) * sizeof(int));
	int index = 0;
	for (i=0;i<4095;i++) {
		if (memsize[i] == 0 && new_block == 0) {
			initial = i;
			holes[index] = initial;
			index++;
			new_block = 1;
		} 
		if (memsize[i] == 0 && memsize[i + 1] == 1) {
			final = i;
			holes[index] = final;
			index++;
			new_block = 0;
		}
	} 
	if (new_block == 1) {
			final = 4095;
			holes[index] = final;
			index++;
		}
	return holes;
} 

void best_fit(int blocksize_num) {
	int i;
	int j;
	int num_of_loop = 2**20/number_of_process;

	for (i=0;i<num_of_loop;i++)
	{
		for (j=0;j<number_of_process;j++) {
			int t = abs((rand() % 90)) + 10; 
			if (t < blocksize_num) {
				blocksize_num = blocksize_num - t;
			}
			else {
				break;
			}
		}
	}
}