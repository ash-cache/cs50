# Usage
## mario.c
```
$ ./mario
Height: 4
   #  #
  ##  ##
 ###  ###
####  ####
```
This was the first program I wrote in C to practice loops. It prompts the user for a height and prints two half pyramids.

# credit.c
```
$ ./credit
Number: 4003600000000014
VISA
```
This was the second program I wrote in C and helped me get used to more concepts like the limitations of C data types and problem solving with code.

I wrote an implementation of the [Luhn Algorithm](https://en.wikipedia.org/wiki/Luhn_algorithm) which is a checksum to determine whether a given string of digits comprises a valid Visa, Mastercard, or American Express credit card number.

These are the steps to perform the checksum (source: [CS50x](https://cs50.harvard.edu/x/2022/psets/1/credit/)):
1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
2. Add the sum to the sum of the digits that weren’t multiplied by 2.
3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
