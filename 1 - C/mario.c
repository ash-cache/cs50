/* In World 1-1 of Super Mario Brothers, Mario jumps over adjacent half pyramids.
   Let's generate those pyramids programatically, representing the blocks with hashes. */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt the user for the height of the pyramids.
    int height;
    do
    {
        height = get_int("Height: ");
        if (height >= 1 && height <= 8)
        {
            break;
        }
    }
    while(true);

    // Number of hashes in the top row is always 1.
    int hashes = 1;
    // Number of spaces in the top row is 'hashes'-many less than the height of the pyramid.
    int spaces = height - hashes;

    // Print pyramids
    for (int i = 0; i < height; i++, spaces--, hashes++)
    {
        for (int j = 0; j < spaces; j++)
        {
            printf(" ");
        }
        // Print first set of hashes
        for (int k = 0; k < hashes; k++)
        {
            printf("#");
        }

        // Print 2 spaces to separate the half pyramids
        printf("  ");
        // Print second set of hashes
        for (int l = 0; l < hashes; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}
