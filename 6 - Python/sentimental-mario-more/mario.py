# The Python version of the first program I wrote in C. It is more concise.
# Prompt user for a height and print two half pyramids with that height

from cs50 import get_int

# Prompt the user for the height and re-prompt while input is invalid
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

# Number of spaces as a function of the height
num_spaces = height - 1
# Number of blocks as a function of number of spaces and the height
num_blocks = height - num_spaces

# Do the following "height" times
for i in range(height):
    print(" " * num_spaces, end="")
    # Decrement the number of spaces for the next line
    num_spaces = num_spaces - 1
    print("#" * num_blocks, end="")
    print("  ", end="")
    print("#" * num_blocks, end="")
    # Update the number of blocks as a function of the number of spaces and the height
    num_blocks = height - num_spaces
    # Print a new line
    print("")
