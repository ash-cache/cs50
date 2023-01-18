# Identify individuals using DNA samples.
# This program demonstrates some data analysis utilities of Python and working with CSV files in particular.
#
# Usage: ./dna.py databases/large.csv sequences/#.txt
#
# Some portions of the human genome contain higher diversity than others.
#
# These sections contain Short Tandem Repeats (STRs) which are a pattern
# of nucleotides, (A, T, C, and G), which repeat consecutively for some length.
#
# The length of the repeats for a given STR is characteristic of a person's DNA.
# It's possible that two people could have the same length for a single STR (P = .05 - .2). However, by
# looking at multiple STRs, we can obtain a signature with which we can match a
# DNA sample with an individual with a very high degree of confidence.
#
# This program looks at 8 STRs.

import csv
import sys

def main():

    # Check for command-line usage
    # argv[0] is the program name, assume argv[1] is a valid .csv file, and argv[2] is a valid .txt file
    if len(sys.argv) != 3:
        print("Usage: python dna.py database.csv textfile.txt")

    # Read database file into a variable
    # Using "with open as file" will automatically close file when finished.
    dbfile = open(sys.argv[1], "r")

    # Read CSV as a list of dictionaries
    dbreader = csv.DictReader(dbfile)

    # Read DNA sequence file into a variable
    # Open returns txtfile as a file object in text mode by default and the read() method returns a string
    txtfile = open(sys.argv[2], "r")
    sequence = txtfile.read()

    # Find longest match of each STR in DNA sequence
    # Create a key-value store (dict) for this sequence using STRs as keys and longest match as values
    signature = {}
    # List of STRs
    STR = ["AGATC","TTTTTTCT","AATG","TCTAG","GATA","TATC","GAAA","TCTG"]

    # Populate signature by counting longest match for each subsequence
    for subsequence in STR:
        count = longest_match(sequence,subsequence)

        # Add the subsequence as a key and count as the associated value
        # Cast the value to a string so we can perform a comparison later
        signature[subsequence] = str(count)

    # Check database for matching profiles
    # Iterate through each person in the database
    for row in dbreader:

        # Suppose that person is a match
        found_match = True

        # The row dict contains elements not in the signature (e.g. "name").
        for item in row:

            # Compare only those elements (subsequences) that are also in the signature
            if item in signature:

                # If the SRT length in the signature is the same as what's in the database row
                # then keep checking the other subsequences in the row.
                if signature[item] == row[item]:
                    continue

                # Otherwise, there's no reason to keep checking. Break out of the loop.
                else:
                    found_match = False
                    break

        # If all the subsequences in this row have matching corresponding values in the signature
        # then found_match remains true. Print the person's name, close open files, and return from the main function.
        if found_match == True:
            name = row["name"]
            print(f"{name}")
            txtfile.close()
            dbfile.close()
            return

        # Otherwise, found_match is false. Go to the next person.
        else:
            continue

    # If no match was found after looping through all the names in the database
    print("No match")
    txtfile.close()
    dbfile.close()
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
