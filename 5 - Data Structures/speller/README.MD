Arrays enable us to **access** data stored in an array in O(1) time. However, arrays have fixed size and if we need to **insert** data when an array is full, we have to allocate enough memory for a new array and copy the values from the first array before we can insert the new value, all of which takes O(n) time. The same is true for **deletion**.

We can get around this by using linked lists where elements are chained so that each element includes a pointer to the next element. We can even make arrays of linked lists, which leads us to a useful data structure called a hash table.

A hash table takes an input key and, using a one-way hash function, produces a number which represents the index in an array (the hash table) where the key-value pair is stored. 

It's possible that two values hash to the same number and this is called a collision. Collisions can be resolved using closed hashing (if a bucket is full, look for the next empty bucket and insert the value there) or open hashing (each bucket contains the head of a linked list, follow the linked list to the end of the chain and insert the value there).

With a good enough hash function, access, insertion, and deletion can be performed in O(1) time.

Most programming languages have built-in functions to create hash tables using an optimized hash function. This program required writing my own hash function and making my own hash table. 


# Writing a good hash function

A good hash function has two main characteristics: 
* It is deterministic (a given input value will always produce the same output)
* It minimizes collisions (the type of information being hashed should not affect the output so that patterns aren't created)

There are a number of techniques used in well-known hash functions:
1. Using bitwise arithmetic
2. Folding (the key to be hashed is divided into parts and "folded" together - the parts are operated on (e.g. addition, multiplication) together.
3. Modulo by a prime number

Shifting a binary number left by n digits is equivalent to multiplying it by 2^n. Shifting it n digits right is equivalent to dividing it by 2^n. The shift operators, `<<` and `>>`, are much faster than their equivalent arithmetic operators (`*`and `/`).

Folding a key divides the key into parts and gives equal weight to each part for affecting the output. This reduces patterns that might emerge based on the type of data (e.g. hashing a list of movie titles where many of them start with "The").

## Modulo by a prime number (Why the length of the hash table should be a prime number)

The goal of a hash table is to enable fast lookup. If our hash table is sufficiently large or there is a poor distribution of hashes, then access starts to become slower. So we don't want our hash table to be too large. How big should it be? One technique that is used is setting the size of the hash table to be a prime number.

A hash function can produce numbers that are larger than the length of the hash table. To make the numbers fit within the hash table, we can compute the modulo of the number with the table length which will produce integers between (0 and (length - 1)).

A problem emerges when dividing by a non-prime number. Integers that are a multiple of one of the factors of the non-prime number will yield the same remainder after modulo with the table length. 

Let's clarify that: Suppose the table length is 14 which has two factors, 7 and 2. All numbers that are a multiple of 7 but not of 14 (e.g. 21, 35, 49) will have the same result after modulo: (21 % 14) == (35 % 14) == (49 % 14) = 7. This results in clusters of data at index 7 which will slow down lookups in the hash table.

We can minimize this type of collision by using a prime number length for the hash table. Prime numbers have only 1 and the number itself as factors.

---

I wanted to find a good hash function to use in this problem. Since I didn't know how to choose a "good" hash function, I referred to the paper, Performance of the most common non‐cryptographic hash functions (Estébanez, C., Saez, Y., Recio, G. and Isasi, P. (2014)) as a place to start. I learned about the [Avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect) and how it's important for creating a good distribution of hash values. lookup3 seemed to be a good candidate while BuzHash could be a unique hash function to test later. CityHash and SpookyHash are good options for x86-64 architecture (which seems to be a safe bet for what AWS is using) according to https://blog.reverberate.org/2012/01/state-of-hash-functions-2012.html.

## Macros vs Functions
Macro functions are preprocessed by the C preprocessor while regular functions are compiled by the compiler.

What this means in practice is that when you compile code that contains macros, the C preprocessor will perform textual substitution (think Find & Replace) wherever it sees the macro used. When the program is run, even if the original C code contained a call to a macro function within the `main()` function, there won't be any need to leave `main()`.

When a regular function is called within `main()`, the variables from `main()` are pushed to the stack, the regular function is called, then the variables are popped from the stack before continuing to execute `main()`.

`getc()` can be used as a macro so it has improved performance over `fgetc()` or `fgets()`. Unfortunately since we can't know the dictionary being used until runtime, this doesn't do us any good.

## fgetc() vs fgets()
`fgetc()` reads the file stream byte by byte. `fgets()` reads a specified number of bytes into a buffer until a new line or EOF is reached. The problem is that `fgets()` includes the new line in the buffer and then appends a null terminator (`\0`). We then have to change the new line character to a null terminator.

But even with performing this correction, fgets() is faster than fgetc() according to my research. This source says that `fgets()` is 5x faster than `fgetc()`.

