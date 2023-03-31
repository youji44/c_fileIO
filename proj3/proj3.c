/*
The goal of this project is to give you system programming experience with four file I/O system calls (open, close, read, and
write), UNIX file permissions, and dynamic memory allocation and deallocation in C. In this project, you will implement a
C program that reads in the contents of two input files. The program will then compare the two files, byte-by-byte, write
differences into two output files, and output timings for the program’s two main steps as stated in the project’s requirements.
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// define two step functions by requirements
void step1(char *file1, char *file2);
void step2(char *file1, char *file2);

// main function
int main(int argc, char *argv[])
{

    // Input file names
    char *filename1;
    char *filename2;

    // Argument length checking
    if (argc == 3)
    {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    else
    {
        printf("Usage: proj3.out <file1> <file2>. \n");
        exit(0);
    }

    // Running time calculating variable
    struct timeval start, end;
    double time_taken;

    // Running starting for step1 function
    gettimeofday(&start, NULL);

    // call step1 requirement function
    step1(filename1, filename2);
    
    // Running ending for step1 function
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_usec - start.tv_usec) * 1e-3;
    printf("Step 1 took %lf milliseconds\n", time_taken);

    // Running starting for step2 function
    gettimeofday(&start, NULL);

     // call step2 requirement function
    step2(filename1, filename2);

    // Running starting for step2 function
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_usec - start.tv_usec) * 1e-3;
    printf("Step 2 took %lf milliseconds\n", time_taken);

    return 0;
}

/*
 Step 1: Compare input files one and two. This should be done by accessing 1 byte at a time from each of the files (use
buffers of size 2: one byte to read and the other byte for a null character), comparing them, and writing any byte from
file one that is not equal to the corresponding byte in file two into the file called differencesFoundInFile1.txt. This
entire step should be implemented as a function called step1, and step1 should be called by your program’s main
function.
*/
void step1(char *file1, char *file2)
{

    // input file descriptor
    int input_file1, input_file2;

    // Output file descriptor
    int output_file;

    input_file1 = open(file1, O_RDONLY);
    input_file2 = open(file2, O_RDONLY);

    if (input_file1 < 0 || input_file2 < 0)
    {
        printf("There was an error reading a file.\n");
        exit(0);
    }

    output_file = open("differencesFoundInFile1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (output_file < 0)
    {
        printf("There was an error writing a file.\n");
        exit(0);
    }

    // Get file size of input files -  length
    // size_one, size_two
    off_t current_pos, size_one, size_two;

    current_pos = lseek(input_file1, 0, SEEK_CUR);
    size_one = lseek(input_file1, 0, SEEK_END);
    lseek(input_file1, current_pos, SEEK_SET);

    current_pos = lseek(input_file2, 0, SEEK_CUR);
    size_two = lseek(input_file2, 0, SEEK_END);
    lseek(input_file2, current_pos, SEEK_SET);

    // allocating two buffer for input one, two
    char *buf_one = (char *)malloc(2 * sizeof(char));
    char *buf_two = (char *)malloc(2 * sizeof(char));

    int i, j;
    for (i = 0, j = 0; i < size_one - 1 || j < size_two - 1; i++, j++)
    {
        // In case first file is bigger than
        if (size_one >= size_two)
        {
            // read a byte on buffer
            read(input_file1, buf_one, 1);
            read(input_file2, buf_two, 1);

            buf_one[1] = '\0';
            buf_two[1] = '\0';

            if (buf_one[0] != buf_two[0])
            {
                // writing difference a byte
                write(output_file, buf_one, 1);
            }
        }
        else  // In case first file is smaller than
        {
            if (i <= size_one - 1)
            {
                // read a byte on buffer
                read(input_file1, buf_one, 1);
                read(input_file2, buf_two, 1);

                buf_one[1] = '\0';
                buf_two[1] = '\0';

                if (buf_one[0] != buf_two[0])
                {
                    // writing difference a byte
                    write(output_file, buf_one, 1);
                }
            }
        }
    }

    // dynamically allocated momory free
    free(buf_one);
    free(buf_two);

    // close opened files
    close(input_file1);
    close(input_file2);
    close(output_file);
}

/*
Step 2: Compare input files one and two.
You will read both of the files into two dynamically allocated arrays.
The input arrays should be allocated to be the exact size needed since we don’t want to waste RAM.
You will then compare the two arrays and write any byte from file two that is not equal
to the corresponding byte in file one into a third array (also dynamically allocated),
and then copy that third array into a file called differencesFoundInFile2.txt. This entire step
should be implemented as a function called step2, and step2 should be called by your program’s main function.
*/
void step2(char *file1, char *file2)
{

    // input file descriptor
    int input_file1, input_file2;

    // Output file descriptor
    int output_file;

    input_file1 = open(file1, O_RDONLY);
    input_file2 = open(file2, O_RDONLY);

    if (input_file1 < 0 || input_file2 < 0)
    {
        printf("There was an error reading a file.\n");
        exit(0);
    }

    output_file = open("differencesFoundInFile2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (output_file < 0)
    {
        printf("There was an error writing a file.\n");
        exit(0);
    }

    // Get file size of input files -  length
    // size_one, size_two
    off_t current_pos, size_one, size_two;

    current_pos = lseek(input_file1, 0, SEEK_CUR);
    size_one = lseek(input_file1, 0, SEEK_END);
    lseek(input_file1, current_pos, SEEK_SET);

    // allocate dynamically arrays from input file
    char *buf_one = (char *)malloc((size_one) * sizeof(char));
    read(input_file1, buf_one, size_one);

    current_pos = lseek(input_file2, 0, SEEK_CUR);
    size_two = lseek(input_file2, 0, SEEK_END);
    lseek(input_file2, current_pos, SEEK_SET);

    // allocate dynamically arrays from input file
    char *buf_two = (char *)malloc(size_two * sizeof(char));
    read(input_file2, buf_two, size_two);

    
    // allocate buffer for writing difference byte
    char *diff = (char *)malloc(2 * sizeof(char));

    int i, j;
    for (i = 0, j = 0; i < size_one - 1 || j < size_two - 1; i++, j++)
    {
        if (size_one >= size_two)
        {   // In case first file is bigger than
            if (j <= size_two - 1)
            {

                if (buf_one[i] != buf_two[j])
                {
                    diff[0] = buf_two[j];
                    diff[1] = '\0';

                    // writing a byte with difference
                    write(output_file, diff, 1);
                }
            }
        }
        else // In case first file is small than
        {
            if (buf_one[i] != buf_two[j])
            {
                diff[0] = buf_two[j];
                diff[1] = '\0';

                // writing a byte with difference
                write(output_file, diff, 1);
            }
        }
    }

    // dynamically allocated momory free
    free(buf_one);
    free(buf_two);
    free(diff);

    // close opened files
    close(input_file1);
    close(input_file2);
    close(output_file);
}
