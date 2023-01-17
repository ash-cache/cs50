/* Digital cameras often use FAT file systems. When images
   are deleted, the actual bytes still remain.

   Given some binary "forensic image" of an SD card,
   recover the deleted files.

   We read block by block into a memory buffer that we
   allocate, and look for a JPEG file signature.

   When we find one, we output the data to a new JPEG
   file until we find a new JPEG signature.

   There may be slackspace (partial block usage), but if the SD card is new
   then it would be zeroed out and this won't affect the image.

   Accepts an input file name as a command line argument.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

const int BLOCK_SIZE = 512;

bool JPEGSIG(BYTE *buffer);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Create a pointer to remember filename
    char *infile = argv[1];

    // In order to use I/O functions, we need a file pointer. fopen() provides the file pointer.
    FILE *inptr = fopen(infile, "r");

    // Ensure the file pointer is not a null pointer
    if (inptr == NULL)
    {
        printf("File could not be opened for reading.\n");
        return 1;
    }

    // A file that has been opened using fopen() can be read using fread()
    // The memory card is formatted as FAT which has a block size of 512 B


    // Dynamically allocate 512 B from heap as a buffer
    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK_SIZE);

    // Allocate buffer for the output file name
    char *outfile = malloc(sizeof(char) * 8);

    /*
       fread returns the number of bytes read and zero otherwise.

       The size of the data to be read is 1 byte.
       BLOCK_SIZE is how many of those data elements to read at once.

       Read (1 byte * BLOCK_SIZE) at a time from inptr into buffer (or rather, where buffer points).

       If fread returns zero then there are no more blocks to read and the loop ends.
    */
    // Count of the number of images found
    int n = 0;

    FILE *outptr = NULL;
    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        // If a JPEG signature is encountered
        if (JPEGSIG(buffer) == true)
        {
            // If a file is already open, close it
            if (outptr == NULL)
            {
                // No file open, do nothing
            }
            else if (outptr != NULL)
            {
                 // Close the previous file
                 fclose(outptr);
            }

            // Print a formatted string to the buffer we allocated earlier
            sprintf(outfile, "%03d.jpg", n);

            // Open a file for writing and create a file pointer
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(outptr);
                printf("Could not create %s\n", outfile);
                return 1;
            }
            // Increment n since a file has been opened for writing
            n++;

            // Write this block to the file
            fwrite(buffer, 1, BLOCK_SIZE, outptr);
        }

        else
        {
            // If this is the first image and a JPEG signature hasn't been found
            // continue to the next block of data without doing anything
            if (n == 0)
            {
                continue;
            }
            // Write data from buffer to outptr
            // An open file has a "file position" attribute that keeps track of where the next
            // byte should be written.
            fwrite(buffer, 1, BLOCK_SIZE, outptr);
        }
    }

    // After the while loop exits, the last file opened still needs to be closed
    fclose(outptr);
    // Close the raw file
    fclose(inptr);

    free(buffer);
    free(outfile);

}

bool JPEGSIG(BYTE *buffer)
{
    // Just as the name of an array is a pointer to its first element,
    // we can treat a pointer as the address of the first element in an array.
    // The first three bytes of a JPEG image are 0xff 0xd8 0xff
    // and the fourth byte's first four bits are 1110.

    // 1110 0000 XOR'ed with any number beginning with 1110, for example,
    // 1110 0000 XOR 1110 0001, or 1110 0000 XOR 1110 1111, will always be at most
    // 0000 1111, or 15. 1110 0000 in decimal is 224.

    if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && ((buffer[3] ^ 224) < 16))
    {
        return true;
    }
    else
    {
        return false;
    }
}
