/*
 * Abdalrhman Juber
 * 1211769
 * section ONE
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define NUM_THREADS 1
#define MATRIX_SIZE 100
/* These data are shared by threads */
typedef struct {
    int startRow;
    int endRow;
} ThreadData;
int A[MATRIX_SIZE][MATRIX_SIZE];
int B[MATRIX_SIZE][MATRIX_SIZE];
int Product[MATRIX_SIZE][MATRIX_SIZE];
void matrix_Initilization(int rows, int cols, int a[rows][cols],int b[rows][cols]){
    for(int i = 0; i < rows;i++)
        for(int j = 0; j <cols ;j++){
            //Student Number 1211769
            a[i][j] = 1;
            if (++j < cols) a[i][j] = 2;
            if (++j < cols) a[i][j] = 1;
            if (++j < cols) a[i][j] = 1;
            if (++j < cols) a[i][j] = 7;
            if (++j < cols) a[i][j] = 6;
            if (++j < cols) a[i][j] = 9;
        }

    for(int i = 0; i < rows;i++)
        for(int j = 0; j <cols ;j++){
            //Student Number 1211769 * 2003 = 2,427,173,307
            b[i][j] = 2;
            if (++j < cols) b[i][j] = 4;
            if (++j < cols) b[i][j] = 2;
            if (++j < cols) b[i][j] = 7;
            if (++j < cols) b[i][j] = 1;
            if (++j < cols) b[i][j] = 7;
            if (++j < cols) b[i][j] = 3;
            if (++j < cols) b[i][j] = 3;
            if (++j < cols) b[i][j] = 0;
            if (++j < cols) b[i][j] = 7;
        }
}
void matrix_multiplication(int rows, int cols, int a[rows][cols],int b[rows][cols] ,int product[rows][cols]){

    int sum = 0;
    for(int i = 0 ; i < rows;i++){
        for(int j = 0 ; j < cols;j++){
            for(int k = 0 ; k < rows;k++){
                sum += a[i][k] * b[k][j];
            }
            product[i][j] = sum;
            sum = 0;
        }
    }

}
void childProcess(int numChild , int pipe_fd[2],int a[MATRIX_SIZE][MATRIX_SIZE],int b[MATRIX_SIZE][MATRIX_SIZE] ,int product[MATRIX_SIZE][MATRIX_SIZE]){
    for(int i = 0; i < numChild;i++){
        pid_t child = fork();

        if(child == -1){//Error occurred
            fprintf(stderr, "Fork Failed");
            exit(EXIT_FAILURE);
        }
        if(child == 0){//Child Process
            //Divide the area into tasks so that every child can do one of them !!
            int rowPerChild = MATRIX_SIZE / numChild;
            int startRow = i * rowPerChild;
            int endRow = (i+1) * rowPerChild;
            int sum = 0;
            close(pipe_fd[0]);
            // Perform matrix multiplication for the assigned rows
            for (int row = startRow; row < endRow; ++row) {
                for (int col = 0; col < MATRIX_SIZE; ++col) {
                    for (int k = 0; k < MATRIX_SIZE; ++k) {
                        sum += a[row][k] * b[k][col];
                    }
                    product[row][col] = sum;
                    sum = 0;
                }
            }

            write(pipe_fd[1], product[startRow], rowPerChild * MATRIX_SIZE * sizeof(int));
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        // Close the write end of the pipe in the parent
        close(pipe_fd[1]);

        // Read the results from child processes
        for (int i = 0; i < numChild; ++i) {
            int rowPerChild = MATRIX_SIZE / numChild;
            int startRow = i * rowPerChild;

            // Read the computed portion from the pipe
            read(pipe_fd[0], product[startRow], rowPerChild * MATRIX_SIZE * sizeof(int));
        }
        // Close the read end of the pipe
        close(pipe_fd[0]);
    }

}
void threadFunc(void *arg) {
    ThreadData *threadData = (ThreadData *)arg;
    int startRow = threadData->startRow;
    int endRow = threadData->endRow;
    int sum = 0;
    for (int row = startRow; row < endRow; ++row) {
        for (int col = 0; col < MATRIX_SIZE; ++col) {
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                sum += A[row][k] * B[k][col];
            }
            Product[row][col] = sum;
            sum = 0;
        }
    }

    free(threadData);
    pthread_exit(NULL);
}
int main() {
    int a[MATRIX_SIZE][MATRIX_SIZE];
    int b[MATRIX_SIZE][MATRIX_SIZE];
    int product[MATRIX_SIZE][MATRIX_SIZE];
    int product_Child[MATRIX_SIZE][MATRIX_SIZE];

    // A- Naive approach (single process)

    // Record the starting time
    clock_t start_time = clock();

    // Call the function to be measured
    matrix_Initilization(MATRIX_SIZE,MATRIX_SIZE,a,b);
    matrix_multiplication(MATRIX_SIZE,MATRIX_SIZE,a,b,product);

    // Record the ending time
    clock_t end_time = clock();

    // Calculate the elapsed time in seconds
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("A- Naive approach: %f seconds\n", elapsed_time);

    // B- Parallel approach using multiple child processes


    int numChild = 3;
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1){
        fprintf(stderr, "Pipe Failed\n");
        exit(EXIT_FAILURE);
    }

    start_time = clock();

    childProcess(numChild,pipe_fd,a,b,product_Child);


    close(pipe_fd[1]);



    // Wait for all child processes to complete
    for (int i = 0; i < numChild; ++i) {
        wait(NULL);
    }

    end_time = clock();
    double elapsed_time_parallel = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("B- Parallel approach with %d child processes: %f seconds\n", numChild, elapsed_time_parallel);

    // Generate matrices
    matrix_Initilization(MATRIX_SIZE,MATRIX_SIZE,A,B);
    matrix_multiplication(MATRIX_SIZE,MATRIX_SIZE,A,B,Product);
    // C- joining and detaching of threads
    // A- First using joining threads !!
    start_time = clock();


    // Create thread handles
    pthread_t threads[NUM_THREADS];

    // Launch threads
    int rowsPerThread = MATRIX_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        ThreadData *threadData = (ThreadData *)malloc(sizeof(ThreadData));
        threadData->startRow = i * rowsPerThread;
        threadData->endRow = (i + 1) * rowsPerThread;

        if (pthread_create(&threads[i], NULL, (void *)&threadFunc, (void *)threadData) != 0) {
            perror("Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Thread join failed\n");
            exit(EXIT_FAILURE);
        }
    }
    // Record the ending time
    end_time = clock();
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("C- A - joining of threads: with %d Threads %f seconds\n",NUM_THREADS , elapsed_time);

    //B-Using Detached

    start_time = clock();


    // Create thread handles
    pthread_t threadd[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    // Launch threads
    int rowsPerThreadd = MATRIX_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        ThreadData *threadData = (ThreadData *)malloc(sizeof(ThreadData));
        threadData->startRow = i * rowsPerThreadd;
        threadData->endRow = (i + 1) * rowsPerThreadd;

        if (pthread_create(&threadd[i], &attr, (void *)&threadFunc, (void *)threadData) != 0) {
            perror("Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Record the ending time
    end_time = clock();
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("C- B - Detached of threads: with %d Threads %f seconds\n",NUM_THREADS , elapsed_time);


    return 0;
}
