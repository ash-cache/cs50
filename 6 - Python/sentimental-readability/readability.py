# I previously wrote this program in C. This is the same program using Python.
# Returns the Coleman-Liau Reading Index for a string of text.
#
# Uses get_string from the CS50 library instead of input(), per the problem specification.
# Strings are a kind of iterable object in Python. This means that you can create a variable
# to iterate over the string without needing to index into it.
from cs50 import get_string

def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Average number of letters per 100 words
    avgletters = (letters / words) * 100
    # Average number of setnences per 100 words
    avgsentences = (sentences / words) * 100

    # Compute the Coleman-Liau Reading Index
    index = round(0.0588 * avgletters - 0.296 * avgsentences - 15.8)

    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    count = 0

    # Create a variable called char to iterate over the string
    for char in text:
        if char.isalpha() == True:
            count += 1

    return count

def count_words(text):
    count = 0

    # Remove leading and trailing whitespace
    text = text.strip()

    # Create a variable called char to iterate over the string
    for char in text:
        if char.isspace() == True:
            count += 1

    # Count the last word (since there was no space following it)
    count += 1

    return count

def count_sentences(text):
    count = 0

    for char in text:
        if char in [".", "!", "?"]:
            count += 1

    return count

main()