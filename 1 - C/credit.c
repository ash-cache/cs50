/* A valid credit card number can be verified using a checksum algorithm (Luhn's algorithm).
   This program prompts the user for a number and either returns INVALID or verifies the number as valid VISA, MASTERCARD, or AMEX.

   The algorithm:
   Starting with the second to last digit, multiply every other digit by 2. Add the digits of those products together.
   Sum this with the sum of the other digits. If the total's last digit is 0 - that is, the total is
   congruent to 0 modulo 10, then the number is valid.

   Additionally:
   AMEX numbers are 15-digits and begin with 34 or 37.
   Visa numbers are 13 or 16-digits and begin with 4.
   Mastercard numbers are 16-digits and begin with 51, 52, 53, 54, or 55. */

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Prompt user for the number
    long input = get_long("Number: ");

    // Inititalize variables for algorithm
    int sumNotMult = 0;
    int sumEveryOther = 0;
    int product = 0;

    // All AMEX numbers begin with 34 or 37 and are 15 digits
    if ((input >= 3.4e14 && input <= (3.5e14 - 1)) || (input >= 3.7e14 && input <= 3.8e14 - 1))
    {
        // Do the following while there are at least 2 digits remaining
        while (input >= 10)
        {
            // Use % 10 to get the last digit in the sequence
            // Add this to the sum of digits that are not being multiplied by 2
            sumNotMult += input % 10;
            // Since the data type is long, divide by 10 to truncate the last digit
            input = input / 10;
            // Use % 10 on the new number to get the last digit. This was the second to last digit.
            product = 2 * (input % 10);
            // If the product has two digits, add those digits to a sum
            if (product >= 10)
            {
                sumEveryOther += product % 10;
                product = product / 10;
                sumEveryOther += product;
            }
            // Otherwise, add the single digit to the same sum
            else
            {
                sumEveryOther += product;
            }
            // Truncate the last digit again before starting a new loop.
            input = input / 10;
        }

        // Take care of the remaining digit
        if (input != 0)
        {
            sumNotMult += input;
        }

        // If the total modulo 10 is congruent to 0 (i.e. the total ends in 0),
        // then return that this is a valid AMEX number
        if ((sumNotMult + sumEveryOther) % 10 == 0)
        {
            printf("AMEX\n");
            return 0;
        }
        // Otherwise, return that this number is INVALID
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }

    // All MASTERCARD numbers begin with 51, 52, 53, 54, or 55 and are 16 digits
    if ((input >= 5.1e15) && (input <= (5.6e15 - 1)))
    {
        while (input >= 10)
        {
            sumNotMult += input % 10;
            input = input / 10;
            product = 2 * (input % 10);

            if (product >= 10)
            {
                sumEveryOther += product % 10;
                product = product / 10;
                sumEveryOther += product;
            }
            else
            {
                sumEveryOther += product;
            }
            input = input / 10;
        }

        if (input != 0)
        {
            sumNotMult += input;
        }

        if ((sumNotMult + sumEveryOther) % 10 == 0)
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }

    // All VISA numbers begin with 4 and are either 13 or 16 digits
    if (((input >= 4e12) && input <= (5e12 - 1)) || ((input >= 4e15) && (input <= (5e15 - 1))))
        {
            while (input >= 10)
            {
                sumNotMult += input % 10;
                input = input / 10;
                product = 2 * (input % 10);

                if (product >= 10)
                {
                    sumEveryOther += product % 10;
                    product = product / 10;
                    sumEveryOther += product;
                }
                else
                {
                    sumEveryOther += product;
                }
                input = input / 10;
            }


            if (input != 0)
            {
                sumNotMult += input;
            }

            if ((sumNotMult + sumEveryOther) % 10 == 0)
            {
                printf("VISA\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }

    else
    {
        printf("INVALID\n");
        return 0;
    }

}
