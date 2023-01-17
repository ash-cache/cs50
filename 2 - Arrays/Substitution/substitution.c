/* Encrypt a message using a Substition cipher.
   Parse the command line for a key - which maps each letter in the alphabet to a new letter,
   prompt for a plaintext, and return the ciphertext. */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Check that a key was included as a command line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check that the key is valid. It must be 26 chars long.
    int n = strlen(argv[1]);
    if (n != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }

    // Check that the key contains only letters.
    for (int i = 0; i < n; i++)
    {
        if (isalpha(argv[1][i]) == false)
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    // Check that each letter in the key appears exactly once
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("There should only be one of each letter in the key. Duplicate letters were detected.\n");
                return 1;
            }
        }
    }

    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;
    int textlen = strlen(plaintext);
    string key = argv[1];

    // To stay consistent, first convert all chars of the key to uppercase.
    // Chars in C are of type int. Subtracting 64 from each char will convert them to their numerical place in the alphabet.

    for (int i = 0; i < n; i++)
    {
        key[i] = toupper(key[i]);
        key[i] = (key[i] - 64);
    }


    // plaintext[i] - 97 gives the lowercase letter's numerical place in the alphabet. plaintext[i] - 65 does the same
    // for upppercase letters. Subtracting this number from plaintext[i] will give the zeroth letter (which is just an
    // ASCII character that comes before the letter 'a' or 'A'). The key[] array is 0-indexed so if we want the 8th letter
    // in the key, it would be key[7]. We add key[j-1] to the zeroth letter to complete the substitution.
    for (int i = 0; i < textlen; i++)
    {
        if (islower(plaintext[i]))
        {
            int j = plaintext[i] - 96;
            ciphertext[i] = ((plaintext[i] - j + key[j - 1]));
        }
        else if (isupper(plaintext[i]))
        {
            int j = plaintext[i] - 64;
            ciphertext[i] = ((plaintext[i] - j + key[j - 1]));
        }
        else
        {
            continue;
        }
    }

    printf("ciphertext: %s\n", ciphertext);





}
