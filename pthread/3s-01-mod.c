#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> //para incluir estructuras timeval

#define MAX_THREADS 8
//#define VECTOR_SIZE 1000000000
#define VECTOR_SIZE 1000000000

pthread_t tid[MAX_THREADS];

int *array;
int length = VECTOR_SIZE;
int count = 0;
int double_count = 0;
int max_threads = MAX_THREADS;

void *count3s_thread(void *arg) {
	int i;
	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	int length_per_thread = length/max_threads;
	// Cast -> http://stackoverflow.com/questions/1640423/error-cast-from-void-to-int-loses-precision
	//int id = (int)arg;
	int id = *((int*)(&arg));
	int start = id * length_per_thread;
	//printf("\tThread [%d] starts [%d] length [%d]\n",id, start, length_per_thread);
	for (i = start; i < start + length_per_thread; i++) {
		if (array[i] == 3) {
			count++;
		}
	}
	//gettimeofday(&tv2,NULL);
	printf("\tThread [%d] starts [%d] length [%d] - delay %lf\n",id, start, length_per_thread,
		(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
}


void initialize_vector() {
	int i = 0;
	array = (int*) malloc(sizeof(int) * VECTOR_SIZE);
	if (array == NULL) {
		printf("Allocation memory failed!\n");
		exit(-1);
	}
	for (; i < VECTOR_SIZE; i++) {
		array[i] = rand() % 20;
		if (array[i] == 3)
			double_count++;
	}
}

int main(int argc, char* argv[]) {
	int i = 0;
	int err;
	struct timeval tv1, tv2, tv3, tv4;
	clock_t t1, t2;
	if (argc == 2) {
		max_threads = atoi(argv[1]);
		if (max_threads > MAX_THREADS)
			max_threads = MAX_THREADS;
	} else {
		max_threads = MAX_THREADS;
	}
	printf("[3s-01] Using %d threads - tamaño int %d\n",max_threads,sizeof(int));
	// random seed
	// http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
	srand(time(NULL));
	printf("*** 3s-01 ***\n");
	printf("Initializing vector... ");
	fflush(stdout);
	gettimeofday(&tv1, NULL);
	initialize_vector();
	gettimeofday(&tv2, NULL);
	printf("Vector initialized! - elapsed %lf sec.\n",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + 
		(double) (tv2.tv_sec - tv1.tv_sec));
	fflush(stdout);
	//t1 = clock();
	gettimeofday(&tv3,NULL);
	while (i < max_threads) {
		err = pthread_create(&tid[i], NULL, &count3s_thread, (void *)i);
		if (err != 0) 
			printf("[3s-01] Can't create a thread: [%d]\n", i);
		else
			printf("[3s-01] Thread created!\n");
		i++;
	}
	gettimeofday(&tv4,NULL);
	printf("[3s-01] Count by threads %d\n", count);
	printf("[3s-01] Double check %d\n", double_count);
	//printf("[[3s-01] Elapsed time %ld ms\n", (ti2 - t1) / CLOCKS_PER_SEC * 1000);
	//printf("[[3s-01] Elapsed time %f\n", (((float)t2 - (float)t1) / 1000000.0F ) * 1000);
	printf("[[3s-01] Elapsed time %lf sec.\n", (double) (tv4.tv_usec - tv3.tv_usec) / 1000000 +
		(double) (tv4.tv_sec - tv3.tv_sec));
	return 0;
}
