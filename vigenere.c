#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

/*
Vigenere cipher shifts each letter in the string by a corresponding letter in the keyword.
When it runs out of letters in the keyword to shift by, it cycles back to the first letter
of the key.
It needs to keep uppercase and lowercase letters consistent. A previous implementation of this
program turned the key value into a number without accounting for letter case of the key and then
did the case check at the step we perform the cipher. This failed to account for a combination of
uppercase and lowercase letters in the key.

This implementation is much cleaner.

What I've learned:
1) Using modulo to cycle through something repeatedly.
2) Keeping parts of a program separate to make it easier to fix bugs.
3) Using modulo with arrays (which start counting at 0 instead of 1).
4) Creating functions in C.
5) Returning an error code and exiting a function after performing a conditional check.
*/

int keycheck(string key);
int shifteval(string key, int alphacounter);
int main(int argc, string argv[])
{
    if (argc != 2) //Check that there is only 1 command line argument. argc counts the program name as an argument so we check that argc is exactly 2.
    {
        printf("Usage: ./vigenere Key\n");
        return 1;
    }

    string key = argv[1];
    int keylen = strlen(key);

    if (keycheck(key) == 1) //Check that the key is valid (consists of alphabet letters only).
    {
        printf("Invalid Key. Key must only consist of English alphabet letters. Usage ./vigenere Key\n");
        return 1;
    }

    string p = get_string("plaintext: ");

    string c = p;

    /*This will count the number of letters in the string.
    It exists outside the for-loop because the for-loop repeats its actions n times
    going through every element of the array.
    This counter needs to be independent of how many times the for-loop has repeated.
    */
    int j = 0;
    for (int i = 0, n = strlen(p);  i < n; i++)
    {
        if (isalpha(p[i]) != 0)
        {
            int shift = shifteval(key, j); //Calculate the key (an integer) to shift the letter by.
            if (isupper(p[i]) != 0)
            {
                /* We subtract by 'A' so that we can treat the letter of the plaintext as its
                   place value in the alphabet and can apply the shift and modulo by 26.
                   We add back 'A' to convert the new number back to the ASCII decimal value.

                   Likewise for lowercase letters.
                */
                c[i] = ((p[i] - 'A') + shift) % 26 + 'A';

            }
            else if (islower(p[i]) != 0)
            {
                c[i] = ((p[i] - 'a') + shift) % 26 + 'a';
            }

            j++; //Increase letter count by 1.
        }
        else
        {
        }
    }


    printf("ciphertext: %s\n", c);
    return 0;
}
/*
Vigenere cipher shifts each letter in the string by a corresponding letter in the keyword.
When it runs out of letters in the keyword to shift by, it cycles back to the first letter
of the key.
This function takes as input the keyword, the place value of the letter in the string that
we're working with (given by alphacounter, which counts the number of letters in the string)
and gives as output, the shift value of the corresponding letter of the keyword.

It accounts for uppercase and lowercase letters in the key by checking and then subtracting by
'A' or 'a' respectively so that we just have the place value of the letter in the alphabet
to serve as the shift value.

We cycle through the key by using modulo. If key length is 5, then if the number of letters is 6,
6 mod 5 = 1 and we use the first letter of the key.

The difference here is that we're working with arrays which start counting at 0 instead of 1. Fortunately,
we can take advantage of the fact that if both the place value counter and the key letter counter start at 0
this remains consistent.
*/
int shifteval(string key, int alphacounter)
{
    int keylen = strlen(key);
    int k = alphacounter % keylen;
    int c = 0;
    /*As long as alphacounter and keylen both start
    counting at zero. They're consistent under modulo.
    E.g. key = bacon
    plaintxt = lightbulb
    plaintext[4], k = 4 corresponds to t. key is 'n', key_k = 4.
    b in bulb is 6th letter, but k = 5, key_k = 0 = b in bacon.
    Range(k) = 0:4
    */

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
/*
Goes through each element of the key array until it hits the null terminator.
If it encounters a non-alphabetic character, it returns 1 and ends the function.
Otherwise it proceeds to the next element. We use a while loop because we may not
know the key length.
*/
int keycheck(string key)
{
    int i = 0;
    while (key[i] != '\0')
    {
        if (isalpha(key[i]) == 0)
        {
            return 1; //If the element of the key array is not alphabet letter, return 1 and end the function.
        }
        else
        {
            i++;
        }
    }
    return 0;
}
