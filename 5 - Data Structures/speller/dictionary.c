/* Implement a hash table and use it to identify misspelled
   words in a text file.

   Usage: ./speller path/to/dictionary path/to.txt

   If a dictionary is not provided, it uses dictionaries/large
   by default which contains 143,091 words.

   First we read each word in the dictionary and hash it. We use
   closed hashing (aka open addressing) where each bucket in the
   table is the head of a linked list. If a word is hashed and the
   corresponding bucket is occupied, we insert the word at the
   beginning of the linked list.

   Subsequently, we read each word in the text file, hash it, and
   look up the hash in the hash table. If it's not found, then we
   count that word as misspelled.

   Example runtime:
   It takes 0.29 seconds to identify 1614 misspelled words in the
   text of Pride and Prejudice which contains 125,203 words. */

// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h> // Library to utilize linux stat function to get a file's size
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // To get the strlen function

#include "dictionary.h"

// Custom global variables
char *buffer = NULL;
FILE *fstream = NULL;
int wordsloaded = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 7919; // 7919 is just a magic number (a large-ish prime)

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Type const char * is not assignable so create a char array to store the word and copy the chars into it.
    int n = strlen(word);
    char *wordtocheck = malloc(n + 1);
    strcpy(wordtocheck, word);

    // Convert any capitalized letters to lowercase to be consistent with the hash table.
    for (int i = 0; i < n; i++)
    {
        wordtocheck[i] = tolower(wordtocheck[i]);
    }

    // Hash the word to an int. This determines the bucket the word should be in.
    int bucket = hash(wordtocheck);
    // The word may not be in the first node in that bucket. Create a pointer that will traverse the linked list.
    node *trav = table[bucket];

    // If the word field of the node matches wordtocheck, then the word is valid. Free wordtocheck and return true.
    // Otherwise, point trav at the next node. Go to that node and repeat the check.
    // Do this as long as there is another node in the chain.
    while (trav != NULL)
    {
        if (strcmp(trav->word, wordtocheck) != 0)
        {
            trav = trav->next;
        }
        else
        {
            free(wordtocheck);
            return true;
        }
    }

    // If the end of the linked list was reached without finding a match, then the word was not in the dictionary.
    free(wordtocheck);
    return false;
}

// Hashes word to a number
// Comments and citation included at the bottom.
unsigned int hash(const char *word)
{
    long sum = 0, mul = 1;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        mul = (i % 4 == 0) ? 1 : mul << 8;
        sum += word[i] * mul;
    }
    return (int)(labs(sum) % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    struct stat filestats; // stat() requires a `struct stat` as an an arg.
    stat(dictionary, &filestats); // Get file statistics from path given by dictionary and store at address of filestats.
    int size = filestats.st_size; // Get the file's size.
    buffer = malloc(sizeof(char) * size); // Allocate size-many bytes as a buffer.
    if (buffer == NULL)
    {
        printf("Failed to allocate buffer.\n");
        return false;
    }

    // dictionary is a pointer to a char *, i.e. a string, that contains the name of the dictionary to load.
    fstream = fopen(dictionary, "r");
    if (fstream == NULL)
    {
        printf("Failed to open dictionary.\n");
        return false;
    }

    // Read all the chars in the dictionary into buffer in one go.
    fread(buffer, sizeof(char) * size, 1, fstream);


    int ctr = 0;
    int len = 0;
    int start = ctr;
    // Iterate through the buffer as an array, going char by char.
    // Use ctr to reference the array index and stop after the last element, (size - 1).
    while (ctr < size)
    {
        // If a new line character is detected, then we must have reached the end of a word.
        if (buffer[ctr] == '\n')
        {
            // Allocate len many chars worth of bytes and one more for \0.
            char *word = malloc((sizeof(char) * len) + 1);
            if (word == NULL)
            {
                printf("Failed to allocate memory for char array");
                return false;
            }

            // Copy len bytes beginning with start to char array.
            memcpy(&word[0], &buffer[start], sizeof(char) * len);
            // Append the null terminator to the word.
            word[len] = '\0';

            // Hash the word into an int.
            int bucket = hash(word);

            // Make a new node to contain the word.
            node *new = malloc(sizeof(node));
            if (new == NULL)
            {
                printf("Error occurred while malloc'ing a new node\n");
            }

            // Fill in the word and next fields of the new node.
            strcpy(new->word, word);
            new->next = NULL;

            // Beginning with C89, locally declared pointers are guaranteed to be initialized to NULL.
            // If a bucket does not point to a valid node, store the new node's address in its pointer.
            if (table[bucket] == NULL)
            {
                table[bucket] = new;
            }
            else
            {
                // If the bucket is pointing to a node, insert this node at the beginning of the linked list.
                // It's important to insert at the beginning of a linked list. Otherwise it will take O(n) to get to the end of the list for each insertion.
                // It's also important to point the new node to the head of the list before making it the new head. Otherwise, we will orphan the list.
                new->next = table[bucket];
                table[bucket] = new;

            }
            wordsloaded++;
            free(word);
            // The char following the new line is the start of the next word.
            start = ctr + 1;
            len = 0;
        }
        else
        {
            len++;
        }

        ctr++;
    }

    // If the position of the file pointer is the expected number of bytes, then all bytes have been read into buffer
    if (ftell(fstream) == size)
    {
        return true;
    }

    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    bool not_loaded = true;
    // wctr counts the number of words
    int wctr = 0;
    for (int i = 0; i < N; i++)
    {
        // Skip pointers not pointing to a node
        if (table[i] == NULL)
        {
            continue;
        }
        // If a non-null pointer is found, then at least one word was loaded.
        not_loaded = false;

        // If the pointer at the table index is not null, then we have found a word
        wctr++;

        // If the pointer to the next node is not null, follow that pointer and count another word
        node *follow = table[i]->next;
        while (follow != NULL)
        {
            wctr++;
            follow = follow->next;
        }
    }
    return wctr;

    if (not_loaded == true)
    {
        return 0;
    }

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    bool unloaded = false;
    int wordsunloaded = 0;

    // Go through the hash table index by index. If the pointer is not pointing to a node, skip it.
    // Otherwise, free the linked list. To do this, remember the next node in the list and free the node.
    // If there is a valid next node, go to it and repeat.
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        if (head == NULL)
        {
            continue;
        }
        while (head != NULL)
        {
            node *tmp = head->next;
            free(head);
            wordsunloaded++;
            head = tmp;
        }
    }

    // Verify that we've unloaded as many words as we loaded.
    if (wordsunloaded == wordsloaded)
    {
        unloaded = true;
    }

    free(buffer);
    fclose(fstream);

    return unloaded;
}
/*
    Adapted from String Folding hash function from OpenDSA Project:
    https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/HashFuncExamp.html
*/
/* Divides the string into 4-byte chunks and then "folds" them using summation.

   Recall that a char is 8 bits. The binary representation of the first 4 chars are concatenated together (going from right to left)
   to create a 32-bit int.

   The first char is taken as is. The mul term is increased by multiplying by 2^8 for the second, third, and fourth chars.
   When the char is multiplied by mul, this is the same as a bit shift of 8, 16, or 24 bits. When the char is added to sum,
   this is effectively concatenating the 8 bits of that char to the left of the sum.

   i % 4 == 0 for the fifth char and it is summed with the byte of the first char. The sixth char is summed with the byte of the
   second char and so on.

    Example: wordtohash = "quick" == 0111|0001 0111|0101 0110|1001 0110|0011 0110|1011
    sum = 0, mul = 1
    i = 0
        i % 4 == 0, therefore mul = 1;
        sum += 0111|0001 * 1
    i = 1
        i % 4 != 0, therefore mul = mul * 2^8 = 2^8
        sum += s[i] * mul
        sum = 0111|0001 + 0111|0101|0000|0000 = 0111|0101|0111|0001
    i = 2
        i % 4 != 0, therefore mul = mul * 2^8 = 2^16
        sum += s[i] * mul
        sum = 0111|0101|0111|0001 + 0110|1001|0000|0000|0000|0000 = 0110|1001|0111|0101|0111|0001
    i = 3
        i % 4 != 0, therefore mul = mul * 2^8 = 2^24
        sum += s[i] * mul
        sum = 0110|1001|0111|0101|0111|0001 + 0110|0011|0000|0000|0000|0000|0000|0000 = 0110|0011|0110|1001|0111|0101|0111|0001
    i = 4
        i % 4 == 0, therefore mul = 1
        sum += s[i] * mul
        sum = 0110|1011 + 0110|0011|0110|1001|0111|0101|0111|0001 = 0110|0011|0110|1001|0111|0101|1101|1100

Original hash function:
int sfold(const char *s, int m)
{
  long sum = 0, mul = 1;
  for (int i = 0, len = strlen(s); i < len; i++)
  {
    mul = (i % 4 == 0) ? 1 : mul << 8; // Left shift 8 bits is equivalent to multiplication by 256
    sum += s[i] * mul;
  }
  return (int)(labs(sum) % m); // labs is absolute value of a long. Absolute value is used when hashing in case the algorithm returns a negative int.
}
*/