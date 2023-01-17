/* Encrypt a message using Caesar's cipher.
   Parse command line arguments for the key,
   prompt for a plaintext, and return the ciphertext.

   ASCII characters can be represented as integers. By adding an integer
   key to a character, we can shift it.*/

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Check that a key was included as a command line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check that the key is valid

    // Key may be more than one char long, check that all chars are digits
    int n = strlen(argv[1]);
    for (int i = 0; i < n; i++)
    {
        if (isdigit(argv[1][i]) == false)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Key could be negative. Check that the key is a positive integer
    int shift = atoi(argv[1]);
    if (shift < 0)
    {
        printf("Usage: ./caesar key. Key must be a positive integer.\n");
    }

    // Prompt the user for the plaintext
    string plaintext = get_string("plaintext:  ");
    string ciphertext = plaintext;
    int plainlen = strlen(plaintext);

    // Encrypt the plaintext, going char by char
    for (int i = 0; i < plainlen; i++)
    {
        // 'a' is 97, 'b' is 98, etc. Subtracting 96 from the char gives its position in the alphabet.
        // Shift this position then add back 96 to get the integer representation.
        if (islower(plaintext[i]))
        {
            ciphertext[i] = (((plaintext[i] - 96) + shift) % 26) + 96;
        }
        // 'A' is 65, 'B' is 67, etc. Subtracting 64 from the char gives its position in the alphabet.
        // Shift this position then add 64 to get the integer representation.
        else if (isupper(plaintext[i]))
        {
            ciphertext[i] = (((plaintext[i] - 64) + shift) % 26) + 64;
        }
        // If the char is non-alphabetic, do nothing
        else
        {
        }
    }

    // Return the ciphertext
    printf("ciphertext: %s\n", ciphertext);


}
