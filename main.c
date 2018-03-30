#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int number_of_process;
int memsize[4096] = {0};
int num_holes;
int proc_table[1230];
int proc = 0;
int gnum = 0;

int* getholes();
void best_fit(int number_of_process);
int* random_request (int number_of_process);
void allocate_mem (int* updated_holes);
void deallocate_mem(int number_of_process);


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
	/*return the holes array with initial and final index of free memory*/
	int count = 0;
	int i;
	int initial;
	int final;
	/*count number of free memory blocks in the entire memory */
	for (i=0;i<4095;i++) {
		if (memsize[i] == 0 && memsize[i + 1] == 1) {
			count = count + 1;
		}
	} 
	if (memsize[4095] == 0) {
		count = count + 1;
	}

	/*Global count of free memory block*/
	num_holes = count;

	int new_block = 0;
	/*create dynamic array of count * 2 to store initial and end index for each free block of memory*/
	int* holes = (int *) malloc ((count * 2) * sizeof(int));
	int index = 0;
	for (i=0;i<4095;i++) {
		/*store the initial and final index of free memory in the holes array*/
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
	/**/
	int* holes;
	int* proc_req;
	int i;
	int j;
	int k;
	int num_of_loop = (1 << 20) / (number_of_process);
	int retry = 0;

	for (j=0;j<num_of_loop;j++) {
		/*loop in the */
		holes = getholes();
		for (i=0;i<num_holes * 2; i = i + 2) {
			printf("blocknumber %d, start %d, end %d \n", i /2, holes[i], holes[i+1]);
		}
		if (retry!=1) {
			proc_req = random_request(number_of_process);
		}
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

		for (i=0;i<num_holes * 2; i = i + 2) {
			printf("Holes holes, start, end %d, %d \n", holes[i], holes[i+1]);
		}
		

		if (hold_this != -1) {
			allocate_mem(holes);
			retry = 0;
		}
		else {
			deallocate_mem(number_of_process);
			retry = 1;
		}
	}
}

int* random_request (int number_of_process) {
	/*return array of random size of process between 10 to 100KB*/
	int* req_array = (int *) malloc (number_of_process * sizeof(int));
	int i;
	for (i=0;i<number_of_process;i++) {
		int t = (rand() % 90) + 10;
		req_array[i] = t;
	}
	return req_array;
}

void allocate_mem (int* updated_holes) {
	/*set all the bits that process occupies to 1.	
	  fill proc_table array with the index in the memory filled with process and the gnum
	*/
	int i;
	int j;
	int* old_holes = getholes();
	
	for (i=0;i<num_holes * 2; i = i + 2) {
			printf("updated holes, start, end %d, %d \n", updated_holes[i], updated_holes[i+1]);
			printf("holes, start, end %d, %d \n", old_holes[i], old_holes[i+1]);			

		}
	for (i=0;i<num_holes * 2; i = i + 2) {
		for (j=old_holes[i]; j<updated_holes[i]; j++) {
			memsize[j] = 1;
		}		
		proc_table[proc] = old_holes[i];
		proc_table[proc + 1] = updated_holes[i];
		proc_table[proc + 2] = gnum;
		proc = proc + 3;
		// printf("Proc %d \n", proc);
		// printf("Gnum %d \n\n", gnum);
	}	
	gnum = gnum + 1;
	// printf("Gnum %d \n", gnum);
	// for (i=0;i<proc;i = i + 3){
	// 	printf("proctor start, end, gnum %d, %d, %d", proc_table[i], proc_table[i+1], proc_table[i+2]);
	// }

}

void deallocate_mem(int number_of_process) {
	int i;
	int j;

	int oldest_group = proc_table[2];
	int numdalloc = 0;
	printf("Gnum is %d, %d \n", gnum, oldest_group);
	// for (i=0; i < proc; i = i + 3) {
	// 	printf("Start, end, gnum, %d, %d, %d \n", proc_table[i], proc_table[i+1], proc_table[i+2]);
	// }
	for (i=0;i<proc;i = i + 3) {
		if (proc_table[i+2] != oldest_group) {
			int k;
			for (k = 0; k < proc; k++) {
				proc_table[k] = proc_table[k + (numdalloc * 3)];
			}
			printf("Proc %d \n", proc);
			break;
		}
		else {
			numdalloc =+ 1;
			for (j = proc_table[i]; j<proc_table[i+1];j++) {
				memsize[j] = 0;
			} 
			proc = proc - 3;
			printf("Proc2 %d \n", proc);
		}	

	}
}