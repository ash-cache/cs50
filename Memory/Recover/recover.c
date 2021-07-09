#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cluster size of FAT16 (up to 32 MiB) and FAT32 (up to 64 MiB) is 512B
   Assumes the memory card was zeroed out and that the JPEGs are contiguous in memory
   Zeroed out memory means that any block either contains JPEG data or zeroes in slack space. Zeroes won't change the image.
*/

// Function prototypes
bool JPEGsig(unsigned char* buffer);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover <file.raw>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");                                           // *file is a file pointer (which is required for stdio functions)

    if (!file)                                                                  // In other words, if fopen, which is pointed to by *file, returns NULL (due to error)
    {
        printf("The file could not be read.");
        return 1;
    }

    int count = 0;
    char filename[8];
    FILE *newJPEG = NULL;                                                       // In order to be able to close an open JPEG later, we have to declare the pointer here.
    unsigned char *buffer = malloc(sizeof(unsigned char) * 512);                // Allocate 512 bytes of memory on the heap that we'll use a buffer into which we'll read the file.

    while (fread(buffer, sizeof(unsigned char), 512, file) == 512)              // fread returns the number of bytes read. If end of file is reached, it returns a short item count or zero.
    {
        //fread(buffer, sizeof(unsigned char), 512, file);

        if (JPEGsig(buffer))
        {
            sprintf(filename, "%03i.jpg", count);                               // sprintf() prints to a string. %3i means a minimum width of 3, and the 0 in front means to zero-fill the number.
            printf("%s\n", filename);

            if (count == 0)
            {
                newJPEG = fopen(filename, "w");
                fwrite(buffer, sizeof(unsigned char), 512, newJPEG);
            }

            else
            {
                fclose(newJPEG);
                newJPEG = fopen(filename, "w");
                fwrite(buffer, sizeof(unsigned char), 512, newJPEG);
            }
            count++;
        }

        else
        {
            if (count == 0)
            {
                continue;
            }
            else
            {
                fwrite(buffer, sizeof(unsigned char), 512, newJPEG);            // If this is not the first time fwrite() is called on a file, it will append to the file instead of overwriting by default.
            }
        }

    }
    free(buffer);                                                               // Free buffer from heap memory.
}



// * I learned bitwise arithmetic
// Check for JPEG signature
// If the file is a JPEG, buffer[3] should be between 0xe0 and 0xef (1110 0000 through 1110 1111).
// We can take the bitwise AND of buffer[3] and 0xe0 (1110 0000) to check for this condition.
// We could also do the condition (buffer[3] & 0xf0 == 0xe0) and get the same result.
bool JPEGsig(unsigned char *buffer)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xe0) == 0xe0))
    {
        return true;                                                            // If the first four bytes of the buffer block match the JPEG signature, it might be a JPEG. Return 'true'.
    }

    return false;                                                               // Otherwise return 'false'.
}
