In this assignment, I applied filters to bitmap images (à la Instagram) to explore how C handles memory. I applied grayscale, sepia, and blur filters. I also reflected the image and applied an edge detection algorithm to it.

![](https://github.com/akcode2/cs50/blob/master/Memory/Images/filteredimages.png)

## Grayscale / Sepia
24-bit bitmap images have 8-bit RGB channels that can range from 0 to 255. If the Red, Green, and Blue channel values are equal, then the result is a shade of gray. The image is converted to grayscale by setting the RGB values to the average of the channels. Sepia is accomplished by computing new RGB values based on the original.

## Blur
I used a box blur filter to create a softening effect. Box blur works by setting each pixel's value to the average of its neighboring pixels (in a 3x3 grid).

## Reflection
Since a bitmap image is just an array of pixels, I iterated over the image row by row and swapped the pixels on the left half and right half.

## Edge detection
I used the Sobel edge detection algorithm. 

For each channel of each pixel, I calculated a new value by applying the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator).

The Sobel operator consists of two kernels, one for detecting edges in the x direction and one for the y direction.

By multiplying the neighboring 3x3 pixels by the respective kernel, we can compute a weighted sum of changes in each direction.

The aggregate of this highlights the edges.
