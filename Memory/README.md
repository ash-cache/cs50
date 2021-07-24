#### Table of Contents
* [Filter (BMP image transformation)](https://github.com/akcode2/cs50/tree/master/Memory#Filter)

* [Recover (JPEG image forensics)](https://github.com/akcode2/cs50/tree/master/Memory#Recover)

# Filter
In this assignment, I applied filters to bitmap images (à la Instagram) to explore how C handles memory. I applied grayscale, sepia, and blur filters. I also reflected the image and applied an edge detection algorithm to it.

![](https://github.com/akcode2/cs50/blob/master/Memory/Images/filteredimages.png)

### Grayscale / Sepia
24-bit bitmap images have 8-bit RGB channels that can range from 0 to 255. If the Red, Green, and Blue channel values are equal, then the result is a shade of gray. The image is converted to grayscale by setting the RGB values to the average of the channels. Sepia is accomplished by computing new RGB values based on the original.

### Blur
I used a box blur filter to create a softening effect. Box blur works by setting each pixel's value to the average of its neighboring pixels (in a 3x3 grid).

### Reflection
Since a bitmap image is just an array of pixels, I iterated over the image row by row and swapped the pixels on the left half and right half.

### Edge detection
I used the Sobel edge detection algorithm. 

For each channel of each pixel, I calculated a new value by applying the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator).

The Sobel operator consists of two kernels, one for detecting edges in the x direction and one for the y direction.

By multiplying the neighboring 3x3 pixels by the respective kernel, we can compute a weighted sum of changes in each direction.

The aggregate of this highlights the edges.

# Recover
The task in this assignment was to recover JPEGs that were deleted from an SD card. I was given a raw memory dump and I wrote a program that read the blocks of memory block-by-block to identify and restore the files.

* I learned that JPEG files contain a header with a "signature" and I learned a bit of bitwise arithmetic to identify this signature. 

* I learned that FAT16 and FAT32 file systems read and write memory in 512 byte blocks (up to a point) and that because of this there is "slack space" when files are stored in memory. It would be interesting to explore the digital forensics implications of that. 

* I learned about file pointers and how to use them with stdio functions.

* I learned not to use `while(!feof)` to check that I haven't reached the end of a file. This is because if we're at `EOF` (last byte of the file), `feof` will return false, the loop will execute, and then on the *next* iteration, `feof` will return true. As a result, the code executes one more time than intended. Instead a better way is to use `while(fread() == nmemb)`. `fread` will return the number of items read and will return a short item count or zero if it reaches `EOF`. This way, we're reading and checking for `EOF` check in the same iteration and will not read past `EOF`.

(edited to add)
According to [Ruslan Spivak](https://ruslanspivak.com/eofnotchar/), EOF is not a character at the end of the file. There is no EOF character at the end of the file. 
>EOF(end-of-file) is a condition provided by the kernel that can be detected by an application. (if k is the current file position and m is the size of a file, performing a read() when k >= m triggers the condition).



