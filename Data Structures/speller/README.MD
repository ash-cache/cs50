I wanted to find a good hash function to use in this problem. Since I didn't know how to choose a "good" hash function, I referred to the paper, Performance of the most common non‐cryptographic hash functions (Estébanez, C., Saez, Y., Recio, G. and Isasi, P. (2014)) as a place to start. I learned about the [Avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect) and how it's important for creating a good distribution of hash values. lookup3 seemed to be a good candidate while BuzHash could be a unique hash function to test later. CityHash and SpookyHash are good options for x86-64 architecture (which seems to be a safe bet for what AWS is using) according to https://blog.reverberate.org/2012/01/state-of-hash-functions-2012.html.

## Macros vs Functions
Macro functions are preprocessed by the C preprocessor while regular functions are compiled by the compiler.

What this means in practice is that when you compile code that contains macros, the C preprocessor will perform textual substitution (think Find & Replace) wherever it sees the macro used. When the program is run, even if the original C code contained a call to a macro function within the `main()` function, there won't be any need to leave `main()`.

When a regular function is called within `main()`, the variables from `main()` are pushed to the stack, the regular function is called, then the variables are popped from the stack before continuing to execute `main()`.

`getc()` can be used as a macro so it has improved performance over `fgetc()` or `fgets()`. Unfortunately since we can't know the dictionary being used until runtime, this doesn't do us any good.

