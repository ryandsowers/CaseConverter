// -------------------------------------------------------------
// File: mycopy.c
//
// Description: this program takes in two file inputs. It 
// attempts to open the first file as a source file with 
// read privileges. The program attempts to open the second
// file as a destination file with write privileges. If these
// two things are accomplished, the program reads from the 
// source file, counting the number of characters copied, the 
// number of characters changed to uppercase, the number of 
// lines in the file, and the number of punctuation characters.
// It then writes the uppercased output to the destination file.
// Each of the counts is printed to the terminal window.
//
// Syntax:
//      Run the program with "./mycopy" followed by a source 
//      file to read from and then a destination file to write
//      to. Ensure that the source file exists first and that 
//      the destination file does not exist.
// -------------------------------------------------------------

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define NUM_VALID_ARGS 3
#define SOURCE 1
#define DEST 2
#define BUFSIZE 128
#define ERROR -1
#define BYTES 1

int main(int argc, char const *argv[])
{
        int fileNameComp;
        char buf[BUFSIZE];
        int charsCopied = 0;
        int charsChanged = 0;
        int numLines = 0;
        int puncChars = 0;
        struct stat metadata;
        size_t num;
        FILE *fd = NULL;
        FILE *fd2 = NULL;

        // check for valid number of arguments entered
        if (argc != NUM_VALID_ARGS) {
                fprintf(stderr, "ERROR. Expected three inputs: ./mycopy 'source file' 'destination file'\n");
                exit(ERROR);
        }

        // check that source and dest are not the same
        fileNameComp = strcmp(argv[SOURCE], argv[DEST]);
        if (!fileNameComp) {
                fprintf(stderr, "ERROR. Source and destination files cannot be the same.\n");
                exit(ERROR);
        }

        // Check if the destination file exists by trying to get the metadata
        stat(argv[DEST], &metadata);
        if (!errno) {
                fprintf(stderr, "ERROR. Destination file already exists. Prevented overwriting.\n");
                exit(ERROR);
        }

        // Try to open source file with READ access
        errno = 0;
        fd = fopen(argv[SOURCE], "r");
        if (errno) {
                perror("ERROR. Tried to open source file with READ access");
                exit(ERROR);
        }

        // Try to open dest file with WRITE access
        errno = 0;
        fd2 = fopen(argv[DEST], "w");
        if (errno) {
                perror("ERROR. Tried to open dest file with WRITE access");
                if (fd != NULL) {
                fclose(fd);
                fd = NULL;
                }
                exit(ERROR);
        }
        
        // convert read text to uppercase and copy to dest file
        while(!feof(fd)) {
                num = fread(buf, BYTES, BUFSIZE, fd);

                for (int i = 0; i < num; ++i) {

                        charsCopied++;

                        if (islower(buf[i])) {
                                charsChanged++;
                        }
                        if (buf[i] == '\n') {
                                numLines++;
                        }
                        if (ispunct(buf[i])) {
                                puncChars++;
                        }

                        buf[i] = toupper(buf[i]);
                }
                fwrite(buf, BYTES, num, fd2);
        }

        printf("Number of characters copied  = %d\n", charsCopied);
        printf("Number of characters changed = %d\n", charsChanged);
        printf("Number of lines in the file  = %d\n", numLines);
        printf("Number of punctuation chars  = %d\n", puncChars);

        // close files
        if (fd2 != NULL) {
                fclose(fd2);
                fd2 = NULL;
        }

        if (fd != NULL) {
                fclose(fd);
                fd = NULL;
        }

        return 0;
}


















