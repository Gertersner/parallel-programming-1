/*
Blake Cruz
Project 1
Parallel and Distributed Computing
Section W03
*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 10240
#define NUM_THREADS 10

int n1, n2;
char *s1, *s2;
FILE *fp;
int countArray[NUM_THREADS] = {0};

// read input file and generate string s1/s2 and length n1/n2
int readf(FILE *fp)
{
    if ((fp = fopen("strings.txt", "r")) == NULL)
    {
        printf("ERROR: can't open string.txt!\n");
        return 0;
    }
    s1 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory!\n");
        return -1;
    }
    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory\n");
        return -1;
    }
    /*read s1 s2 from the file*/
    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);
    n1 = strlen(s1);  /*length of s1*/
    n2 = strlen(s2) - 1; /*length of s2*/

    if (s1 == NULL || s2 == NULL || n1 < n2)  /*when error exit*/
        return -1;
    return 0;
}

// start of my logic
int num_substring(int t)
{
    // calculate the proper partion for each thread
    int currentCount = 0;
    int subLength = n1 / NUM_THREADS;

    // start and end of each threads search area
    int begin = t * subLength;
    int end = begin + subLength;

    // loop through each character in the desired area
    for (int i = begin; i<=end - n2; i++)
    {
        int j;
        for (j = 0; j < n2; j++)
        {
            //check if the characters match, if not then break
            if (s1[i + j] != s2[j])
            {
                break;
            }
        }
        //if the substring mathces, increment the count
        if (j == n2)
        {
            currentCount++;
        }
    }

    return currentCount;
}

void *calSubStringThread(void *threadid)
{
    long tid = (long)threadid;
    printf("This is thread %ld, ", tid);
    int num = num_substring(tid);
    printf("find num of is: %d\n", num);
    //add thread count result to this array
    countArray[tid] = num;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
 
    pthread_t threads[NUM_THREADS];
    int t, rc;
    int totalNum = 0;

    readf(fp);

    // start the timer
    start = clock();
  
    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *)(size_t)t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    for (t = 0; t < NUM_THREADS; t++)
    {
        totalNum += countArray[t];
    }
  
    end = clock(); // stop the timer
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  
    printf("The number of substrings is: %d\n", totalNum);
    printf("CPU time used: %f seconds\n", cpu_time_used);
    return 1;
}
