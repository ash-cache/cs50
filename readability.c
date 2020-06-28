#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int index(float letters, float words, float sentences);

int main(void)
{

    string s = get_string("Text: ");
    int letterct =  0;
    int wordct = 0;
    int sentct = 0;
    int gradelvl = 0;
    int length = strlen(s);

    for (int i = 0; i <= length; i++)
    {
        /*
        Check if the ith character in the string array is a letter and if it is, increases the
        letter count by 1. If the character is a space, then we've reached the end of a word
        and we count that. If we encounter a ., ?, or ! then we have reached the end of a
        sentence and we count that.

        Key takeaway: The string array actually has strlen+1 elements. The last element,
        s[i = length], is the null terminator, '\0'.

        If we reach this character instead of a punctuation mark or a space, we will have reached
        the last word and will count this word separately. This lets us count words accurately
        when the passage ends in the middle of a sentence.

        */
        if (isalpha(s[i]) != 0)
        {
            letterct += 1;
        }
        else if (isspace(s[i]) != 0)
        {
            wordct += 1;
        }
        else if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            sentct += 1;
        }
        else if (s[i] == '\0')
        {
            wordct += 1;
        }



    }

    //Calculate grade level using the index function.
    gradelvl = index(letterct, wordct, sentct);


    //Prints the final output.
    if (gradelvl < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (gradelvl > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", gradelvl);
    }
}

//Takes the total number of letters, words, and sentences as input. Then computes the average number
//of letters per 100 words and the average number of sentences per 100 words.
//Then computes the grade level of the text using the Coleman-Liau formula.
int index(float letters, float words, float sentences)
{
    float average_letters = (letters / words) * 100;
    float average_sentences = (sentences / words) * 100;

    return round((0.0588 * average_letters - 0.296 * average_sentences - 15.8));
}
