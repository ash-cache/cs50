# A very fast spellchecker

## The problem with arrays
Why can't we just use an array? We could somehow transform a word into an array index and look it up.

Arrays enable us to access data in O(1) time. However, arrays have fixed size. In order to add more data we have to...
1. Allocate memory for a new array
2. Copy values to the new array
3. Insert the new value
   
All of which takes O(n) time.

## Enter linked lists
A much more efficient alternative is a linked list. A linked list is a data structure made of chains of elements. Every element stores a pointer to the next element in the chain.

An array of linked lists is called a hash table.

## How a hash table works

A hash table takes an input value and, using a one-way hash function, produces a number. That number is the key (i.e. the array index) where that key-value pair is stored. 

## Dealing with hash collisions
It's possible that two values hash to the same number and this is called a collision. Collisions can be resolved using closed hashing or open hashing.

*Closed hashing*: If a bucket is full, look for the next empty bucket and insert the value there

*Open hashing*: Each bucket contains the head of a linked list, follow the linked list to the end of the chain and insert the value there.

## About hash functions

With a good enough hash function, access, insertion, and deletion can be performed in O(1) time.

Most programming languages have built-in functions to create hash tables using an optimized hash function. This assignment required writing my own hash function and making my own hash table.

## Writing a good hash function
A good hash function has two main characteristics:

* It is deterministic: A given input value will always produce the same output.
* It minimizes collisions: The input should not affect the output. This prevents patterns. Patterns make collisions.

In my research I learned that one technique used in well-known hash functions is using bitwise arithmetic.

### Using bitwise arithmetic
* **Folding** 

* **Modulo by a prime number** 

* **Bit shifting**

#### Folding
Folding a key divides the key into parts and gives equal weight to each part for affecting the output. The parts are operated on (e.g. addition, multiplication) together. This reduces patterns that might emerge based on the type of data (e.g. hashing a list of movie titles where many of them start with "The").

#### Modulo by a prime number (Why the length of the hash table should be a prime number)

The goal of a hash table is to enable fast lookup. If our hash table is sufficiently large or there is a poor distribution of hashes, then access starts to become slower. So we don't want our hash table to be too large. How big should it be? One technique that is used is setting the size of the hash table to be a prime number.

A hash function can produce numbers that are larger than the length of the hash table. To make the numbers fit within the hash table, we can compute the modulo of the number with the table length which will produce integers between (0 and (length - 1)).

A problem emerges when dividing by a non-prime number. Integers that are a multiple of one of the factors of the non-prime number will yield the same remainder after modulo with the table length.

Let's clarify that: Suppose the table length is 14 which has two factors, 7 and 2. All numbers that are a multiple of 7 but not of 14 (e.g. 21, 35, 49) will have the same result after modulo: (21 % 14) == (35 % 14) == (49 % 14) = 7. This results in clusters of data at index 7 which will slow down lookups in the hash table.

We can minimize this type of collision by using a prime number length for the hash table. Prime numbers have only 1 and the number itself as factors.
#### Bit shifting

Shifting a binary number left by n digits is equivalent to multiplying it by 2^n. Shifting it n digits right is equivalent to dividing it by 2^n. The shift operators, << and >>, are much faster than their equivalent arithmetic operators (*and /).

