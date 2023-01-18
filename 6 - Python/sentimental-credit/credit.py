# I implemented the same checksum using Luhn's Algorithm that I previously wrote in C, using Python.
# Using methods like str.startswith() and string slicing techniques, I was able to do this with fewer lines of code.

def main():
    number = input("Number: ")
    n = len(number)

    # If the string length isn't 13, 15, or 16 then immediately conclude the number is invalid
    if n == 13:
        if number.startswith("4"):
            # Possibly a valid number
            if checksum(number):
                print("VISA")
            else:
                print("INVALID")
        else:
            print("INVALID")

    elif n == 15:
        if number.startswith("34") or number.startswith("37"):
            if checksum(number):
                print("AMEX")
            else:
                print("INVALID")
        else:
            print("INVALID")

    elif n == 16:
        # These are possible starting digits for Mastercard
        MCstring = ["51", "52", "53", "54", "55"]
        if number.startswith("4"):
            # Possibly a valid number
            if checksum(number):
                print("VISA")
            else:
                print("INVALID")
        # Slice the first 2 chars from the string - that is, index 0 and 1.
        # Check if this string slice is in the list of starting strings for Mastercard
        elif number[0:2] in MCstring:
            # Possibly a valid number
            if checksum(number):
                print("MASTERCARD")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")

def checksum(number):
    sum_second_last = 0
    sum_last = 0

    # Slice the string. Start at the second to last digit (-2) and go to the first. -2 steps from right to left by 2.
    for i in number[-2::-2]:
        product = int(i) * 2
        # If the product has more than one digit, sum the digits
        if product > 9:
            # Use // to truncate decimal after division
            first_digit = product // 10
            second_digit = product % 10
            sum_second_last += (first_digit + second_digit)
        else:
            sum_second_last += product

    # Slice the string. Start at the last digit (-1) and go to the first. -2 steps from right to left by 2.
    for i in number[-1::-2]:
        sum_last += int(i)

    sum = sum_second_last + sum_last

    if (sum % 10) == 0:
        return True
    else:
        return False

main()