#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
/*This program peforms a substitution cipher.
It takes as a command line argument, a 26 letter key.
It takes as input, a plaintext string and replaces each letter with
the appropriate letter of the key and outputs the ciphered text.
*/
bool validkey(string key);

int main(int argc, string argv[])
{
    int difference[26];
    string key = argv[1];

    //Check that only one command line argument is included when executing the program.
    //The text to execute the command counts as an argument so total argument count should be 2.
    if (argc != 2)
    {
        printf("Missing command line argument.\n");
        return 1;
    }
    else if (validkey(key) == false)
    {
        printf("Invalid command line argument. Please enter a key consisting of 26 letters\n");
        return 1;
    }

    /*We iterate over the 26 elements of the array. If it's an uppercase letter,
    we find the remainder of that letter's integer value  when divided by 65 (65 = A).

    In other words, the number modulo 65 is congruent to the letter's
    place in the alphabet (when counting from 0).

    i represents the letter of the alphabet that we're comparing over. i = 0 = A, i = 1 = B, etc.
    The difference between the letter and the letter we're comparing is our shift value.
    We record it in the difference array.

    Repeat this for lowercase letters, but with modulo 97 (97 = a).
    */
    for (int i = 0; i < 26; i++)
    {
        //isupper, islower, and isalpha return 0 unless they are true.
        if (isupper(key[i]) != 0)
        {
            difference[i] = key[i] % 65 - i;
        }
        if (islower(key[i]) != 0)
        {
            difference[i] = key[i] % 97 - i;
        }
    }

    string plaintxt = get_string("plaintext: ");
    int len = strlen(plaintxt);

    string ciphertxt = plaintxt;

    int j = 0;
    while (j < len)
    {
        if (isalpha(plaintxt[j]) != 0)
        {
            if (isupper(plaintxt[j]) != 0)
            {
                /*Cast the jth element as an integer. The remainder when this
                is divided by 65, is the letter's numerical place in the
                difference array.
                */
                int k = (int) plaintxt[j] % 65;
                /*The kth element of the difference array tells us how much
                //to shift the jth element of the plaintext array. This is
                the ciphered letter.
                */
                ciphertxt[j] = plaintxt[j] + difference[k];
                j++;
            }
            //Repeat this for lowercase letters modulo 97.
            else if (islower(plaintxt[j]) != 0)
            {
                int k = (int) plaintxt[j] % 97;
                ciphertxt[j] = plaintxt[j] + difference[k];
                j++;
            }
        }
        else
        {
            j++;
        }

    }

    printf("ciphertext: %s\n", ciphertxt);
}

bool validkey(string key)
{


    //If string length is 26, then proceed to check each letter. Otherwise,
    //return false to the function that called validkey.
    if (strlen(key) == 26)
    {
        for (int i = 0; i < 26; i++)
        {
            //Check if the ith element of the array is a letter.
            //If it is, do nothing. If it's not, exit the function returning false.
            if (isalpha(key[i]) != 0)
            {

            }
            else
            {
                return false;
            }
        }
        //This function checks that each letter of the key is not repeated.
        //The first for loop iterates over each letter of the key.
        //The nested while loop checks for matches with the rest of the letters
        //starting with the next letter. Because we're doing this check beginning
        //at the first letter, we only need to check all the "next letters" and
        //don't have to check the preceding letters for matches because they will
        //already have been checked.
        //If a duplicate is found, the function exits and returns false.
        //
        for (int i = 0; i < 26; i++)
        {
            int j = i + 1;
            while (j < 26)
            {
                if (key[i] == key[j])
                {
                    return false;
                }
                else
                {
                    j++;
                }
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}
