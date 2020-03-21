/* Credit - CS50 pset1
/  This program takes a number and performs a checksum to determine if it's a valid VISA, American Express, or MasterCard.
/  It uses Luhn's algorithm: 
/   1) Add up every other digit beginning with the last digit.
/   2) Multiply every other digit by 2 beginning with the second to last digit. Add the digits of these products. Sum these numbers.
/   3) Add both sums together. If the total modulo 10 is congruent to zero, then the number is a valid credit card number.
*/

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Prompt user for the credit card number.
    long long int Number = get_long_long("Number: "); 
    // Every other digit beginning with the last digit
    int last_digit = 0; 
    // Every other digit beginning with the second to last digit
    int other_digit = 0; 
    int product = 0; 
    int total = 0;
    int sum_of_digits = 0;
    // Initialize boolean that will change to 'true' if the checksum is valid
    bool is_valid = false; 
    // Initialize booleans that will check if the number's format indicates AmEx/VISA/MasterCard
    bool AMEXformat = false; 
    bool VISAformat = false;
    bool MASTERCARDformat = false;

    // Check if number looks like a VISA. VISA numbers are 13 or 15 digits long and begin with 4.
    if (((Number >= 4e12 && Number <= (5e12 - 1)) || (Number >= 4e15 && Number <= (5e15 - 1))))
    {
        VISAformat = true;
    }
    // Check if number looks like a MasterCard. MC numbers are 16 digits long and begin with 51, 52, 53, 54, or 55.
    else if (((Number >= 5.1e15 && Number <= (5.2e15 - 1)) || (Number > 5.2e15 && Number < (5.3e15 - 1)) || (Number >= 5.3e15
              && Number <= (5.4e15 - 1)) || (Number >= 5.4e15 &&  Number <= (5.5e15 - 1)) || (Number >= 5.5e15 && Number <= (5.6e15 - 1))))
    {
        MASTERCARDformat = true;
    }
    // Check if number looks like an American Express. AmEx cards are 15 digits long and begin with 35 or 37.
    else if (((Number >= 3.4e14 && Number <= (3.5e14 - 1)) || (Number >= 3.7e14 && Number <= (3.8e14 - 1))))
    {
        AMEXformat = true;
    }
    
    // This while loop cycles through the digits of the credit card number, beginning with the last digit. 
    // It uses division to truncate the digit once we no longer need it. Once we run out of digits, the loop will exit.
    while (Number > 0) 
    {
        // Use Number modulo 10 to obtain the last digit.
        last_digit = Number % 10;
        // Add the last digit to the total.
        total += last_digit;
        // Since Number is an integer, it cannot hold decimal values. By dividing by 10, last digit will become a decimal
        // and will be truncated.
        Number /= 10;
        // Use Number modulo 10 to obtain the second to last digit, which is now the last digit of the number.
        other_digit = Number % 10;
        // Multiply the digit by 2 and use product to hold it.
        product = other_digit * 2;
        // Product can be a one or two-digit number. If it's a two digit number, we use product modulo 10 to get the last digit
        // and we use product divided by 10 to get the first digit (remembering that since product is an integer, the decimal will
        // be lost). If product is a one-digit number, then product divided by 10 will be zero. Add the sum of the digits. 
        sum_of_digits = ((product % 10) + (product / 10));
        // Add the sum of the digits of the product to the total.
        total += sum_of_digits;
        // Divide Number by ten to truncate the last digit again and restart the loop.
        Number /= 10;
    }
    
    // Check if total modulo 10 is congruent to 0. If it is, then the checksum is valid.
    if (total % 10 == 0)
    {
        is_valid = true;
    }
    
    // Check if the Number resembles VISA/MC/AMEX and the checksum is valid. If so, print the card type.
    if (VISAformat == true && is_valid == true)
    {
        printf("VISA\n");
    }

    else if (AMEXformat == true && is_valid == true)
    {
        printf("AMEX\n");
    }

    else if (MASTERCARDformat == true && is_valid == true)
    {
        printf("MASTERCARD\n");
    }
    // For all other cases, return INVALID.
    else
    {
        printf("INVALID\n");
    }

}    

