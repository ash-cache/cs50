/* Compute the Coleman-Liau index of some text
   - that is, determine the reading grade level.

   The formula is: index = 0.0588 * L - 0.296 * S - 15.8
   where L is the avg number of letters and S is the avg number of sentences.

   Prompts the user for a string of text and returns the grade level. */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string s);
int count_words(string s);
int count_sentences(string s);


int main(void)
{
    string s = get_string("Text: ");

    int numLetters = count_letters(s);
    int numWords = count_words(s);
    int numSentences = count_sentences(s);
    int index = 0;

    // The average number of letters and sentences per 100 words is determined using proportions.
    // When dividing two ints, at least one of them must be a double in order for the quotient to be a double.
    double L = (numLetters * 100) / (double) numWords;
    double S = (numSentences * 100) / (double) numWords;

    // Perform the Coleman-Liau Reading Level Index calculation. Round the result (which is a double) and cast it to an int.
    index = (int) round((0.0588 * L) - (0.296 * S) - 15.8);

    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string s)
{
    int letters = 0;

    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z'))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string s)
{
    int words = 0;

    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (s[i] == ' ')
        {
            words++;
        }
    }

    // To account for the fact that there's no space after the final word
    words++;
    return words;
}

int count_sentences(string s)
{
    int sentences = 0;

    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}
