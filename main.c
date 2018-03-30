#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int number_of_process;
int memsize[4096] = {0};
int num_holes;
int* getholes();
void best_fit(int number_of_process);
int* random_request (int number_of_process);

int main (int argc, char** argv) {
	// if more or less than one argument, prints error
	if(argc != 2) {
		printf("Error");
	}

	//converts number of process to integer
	else {
		srand(time(NULL));
		number_of_process = atoi(argv[1]);
		if (number_of_process > 0) {
			best_fit(number_of_process);
		}
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

void best_fit(int number_of_process) {
	int* holes;
	int* proc_req;
	int i;
	int j;
	int k;
	int num_of_loop = 1 << 20/number_of_process;

	for (j=0;j<num_of_loop;j++) {
		holes = getholes();
		for (i=0;i<num_holes * 2; i = i + 2) {
			printf("blocknumber %d, start %d, end %d \n", i /2, holes[i], holes[i+1]);
		}
		proc_req = random_request(number_of_process);
		for (i=0;i<number_of_process;i++) {
			printf("Process %d \n", proc_req[i]);
		}
		int hold_this = -1;
		int hole_size;
		for (i=0;i<number_of_process;i++) {
			hold_this = -1;
			for (k=0;k<num_holes * 2; k = k + 2) {
				hole_size = holes[k + 1] - holes[k];
				if ((proc_req[i] < hole_size) && (hold_this == -1)) {
					hold_this = k;
				}
				if ((hold_this != -1) && ((holes[hold_this+1] - holes[hold_this]) > hole_size)) {
					hold_this = k;
				}
			}
			if (hold_this == -1) {
				break;
			}
			else {
				holes[hold_this] = holes[hold_this] + proc_req[i];
			}
		}

		if (hold_this != -1) {

		}
	}


}

int* random_request (int number_of_process) {
	int* req_array = (int *) malloc (number_of_process * sizeof(int));
	int i;
	for (i=0;i<number_of_process;i++) {
		int t = (rand() % 90) + 10;
		req_array[i] = t;
	}
	return req_array;
}

void allocate_mem (int* updated_holes) {
	int* old_holes = getholes();
	
}