/* Encrypt a message using a Vigenere cipher.

   Parse command line arguments for a keyword, prompt for a plaintext,
   and return the ciphertext.

   The Vigenere cipher shifts each letter in the plaintext by a corresponding 
   letter in the keyword.
   
   When it runs out of letters in the keyword to shift by, it cycles back 
   to the first letter of the key.

   It's important to preserve letter case and only shift letters. */

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int keycheck(string key);
int shifteval(string key, int alphacounter);
int main(int argc, string argv[])
{
     // Check that there is only 1 command line argument. argc counts the program name as an argument so we check that argc is exactly 2.
    if (argc != 2)
    {
        printf("Usage: ./vigenere Key\n");
        return 1;
    }

    string key = argv[1];
    int keylen = strlen(key);

    // Check that the key is valid (consists of alphabet letters only).
    if (keycheck(key) == 1) 
    {
        printf("Invalid Key. Key must only consist of English alphabet letters. Usage ./vigenere Key\n");
        return 1;
    }

    string p = get_string("plaintext: ");

    string c = p;

    // Let j count the number of letters in the plaintext
    int j = 0;
    for (int i = 0, n = strlen(p);  i < n; i++)
    {
        if (isalpha(p[i]) != 0)
        {
            // Determine how much to shift the letter given its place in the plaintext
            int shift = shifteval(key, j); 
            if (isupper(p[i]) != 0)
            {
                /* We subtract by 'A' so that we can treat the letter of the plaintext as its
                   place value in the alphabet and can apply the shift and modulo by 26.
                   We add back 'A' to convert the new number back to the ASCII decimal value.

                   Likewise for lowercase letters. */
                c[i] = ((p[i] - 'A') + shift) % 26 + 'A';

            }
            else if (islower(p[i]) != 0)
            {
                c[i] = ((p[i] - 'a') + shift) % 26 + 'a';
            }
            
            // Increase letter count by 1.
            j++; 
        }
        else
        {
        }
    }


    printf("ciphertext: %s\n", c);
    return 0;
}
/* This function takes as input the keyword, the place value of the letter in the string that
   we're working with (given by alphacounter, which counts the number of letters in the string)
   and gives as output, the shift value of the corresponding letter of the keyword.

   We cycle through the key by using modulo. If key length is 5, then if the number of letters is 6,
   6 mod 5 = 1 and we use the first letter of the key. */
int shifteval(string key, int alphacounter)
{
    int keylen = strlen(key);
    int k = alphacounter % keylen;
    int c = 0;
    /* As long as alphacounter and keylen both start
    counting at zero. They're consistent under modulo.
    E.g. key = bacon
    plaintext = lightbulb
    plaintext[4], k = 4 corresponds to 't'. key is 'n', key_k = 4.
    b in bulb is 6th letter, but k = 5, key_k = 0 = b in bacon.
    Range(k) = 0:4 */


    // Key can contain a mix of uppercase and lowercase letters,
    // we only need the position in the alphabet.
    if (isupper(key[k]) != 0)
    {
        c = key[k] - 'A';
    }
    else if (islower(key[k]) != 0)
    {
        c = key[k] - 'a';
    }
    return c;
}
/* Goes through each element of the key array until it hits the null terminator.
   If it encounters a non-alphabetic character, it returns 1 and ends the function.
   Otherwise it proceeds to the next element. We use a while loop because we may not know the key length.
*/
int keycheck(string key)
{
    int i = 0;
    while (key[i] != '\0')
    {
        if (isalpha(key[i]) == 0)
        {
            // If the element of the key array is not alphabet letter, stop executing and return 1
            return 1; 
        }
        else
        {
            i++;
        }
    }
    return 0;
}
