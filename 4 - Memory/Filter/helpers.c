#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h> // Needed to have the 'bool' identifier 

// Convert image to grayscale
// image is a 2-dimensional array of type RGBTRIPLE (24 bit pixels)
// If R == G == B, then the pixel is a shade of gray.
// To convert to grayscale, we'll set each channel to the average of the 3 channels.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each row
    for (int i = 0; i < height; i++)
    {
        // Iterate over each column
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the channels.
            float total = (image[i][j].rgbtRed +  image[i][j].rgbtGreen + image[i][j].rgbtBlue);
            int avg = (int) round(total / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
// We can do this by simply swapping the pixels on the left half with the pixels on the right.
// Midpt is an int so it will always round down (if width is an odd number). In this case, the center column is left alone.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, midpt = width / 2; j < midpt; j++)
        {
            //swap
            RGBTRIPLE RGBdummy = image[i][j];  // [i][j] begins at left of the row and goes towards the midpoint.
            image[i][j] = image[i][(width - 1) - j]; // [i][(width - 1) - j] begins at the first pixel on the right (width - j) and goes towards the midpoint.
            image[i][(width - 1) - j] = RGBdummy;
        }
    }
    return;
}

// Blur image using a Box Blur.
// We do this by setting each pixel's RGB values to the average of its neighboring pixels
// (within a 1 pixel radius and including the pixel [i][j] itself).
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imgblur[height][width]; // Create an array to store our calculated values

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float redSum = 0, greenSum = 0, blueSum = 0;
            int count = 0;
            float redAvg = 0, greenAvg = 0, blueAvg = 0;

            /*
             These are the pixels we have to concern ourselves with for each pixel [i][j].
             They're included here for clarity, but accomplished using the two for loops below.
             RGBTRIPLE top = image[i - 1][j];
             RGBTRIPLE bottom = image[i + 1][j];
             RGBTRIPLE left = image[i][j-1];
             RGBTRIPLE right = image[i][j+1];
             RGBTRIPLE topleft = image[i-1][j-1];
             RGBTRIPLE topright = image[i-1][j+1];
             RGBTRIPLE bottomleft = image[i+1][j-1];
             RGBTRIPLE bottomright = image[i+1][j+1;]
            */

            /* All pixels within 1 pixel of [i][j] can be expressed as permutations of [i +/- 1][j +/- 1] and [i +/- 1][j] and [i][j +/- 1].
             If we think of this as a 3 x 3 array, we can iterate over this starting with [0][0] and ending with [2][2].
             However, we need to do this for any arbitrary i and j.
             We can express this relationship as c = (i - 1 + m) where m is [0,1,2].
             We should also skip the pixels that are outside of the boundaries of the image.
            */
            for (int m = 0; m < 3; m++)
            {
                int c = (i - 1 + m);
                if (c < 0 || c >= height) // If c would be outside of the image, skip this row.
                {
                    continue;
                }
                for (int n = 0; n < 3; n++)
                {
                    int d = (j - 1 + n);
                    if (d < 0 || d >= width) // If d would be outside of the image, skip this column.
                    {
                        continue;
                    }
                    redSum += image[c][d].rgbtRed;
                    greenSum += image[c][d].rgbtGreen;
                    blueSum += image[c][d].rgbtBlue;
                    count++; // count should only count the pixels that are summed (excluding pixels that are out of bounds).
                }
            }
            // Calculate averages and round them.
            redAvg = round(redSum / count);
            greenAvg = round(greenSum / count);
            blueAvg = round(blueSum / count);

            //Set the pixel's RGB values to the average.
            imgblur[i][j].rgbtRed = redAvg;
            imgblur[i][j].rgbtGreen = greenAvg;
            imgblur[i][j].rgbtBlue = blueAvg;
        }
    }

    // Overwrite image with calculated values.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = imgblur[i][j].rgbtRed;
            image[i][j].rgbtGreen = imgblur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = imgblur[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
/* Apply the Sobel edge detection algorithm to the image.
   For each channel of each pixel, we calculate a new value by applying the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator).
   The Sobel operator consists of two kernels, one for detecting edges in the x direction and one for the y direction.
   By multiplying the neighboring 3x3 pixels by the respective kernel, we can compute a weighted sum of changes in each direction.
   The aggregate of this highlights the edges.
*/
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] =
    {
      {-1, 0, 1},
      {-2, 0, 2},
      {-1, 0, 1}
    };

    int Gy[3][3] =
    {
      {-1, -2, -1},
      {0 , 0, 0 },
      {1 , 2, 1 }
    };

    RGBTRIPLE imgedges[height][width]; // Create an array to store the calculated values.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int xsumRed = 0, ysumRed = 0, xsumGreen = 0, ysumGreen = 0, xsumBlue = 0, ysumBlue = 0;
            int sumSqrdR = 0, sumSqrdG = 0, sumSqrdB = 0;
            int roundsqrtR = 0, roundsqrtG = 0, roundsqrtB = 0;

            for (int m = 0; m < 3; m++)
            {
                int c = (i - 1 + m);

                bool RowOOB = false;
                if (c < 0 || c >= height) // If c would be outside of the image, skip this row.
                {
                    continue;
                }

                for (int n = 0; n < 3; n++)
                {
                    int d = (j - 1 + n);

                    bool ColOOB = false;
                    if (d < 0 || d >= width) // If d would be outside of the image, skip this column.
                    {
                        continue;
                    }
                    xsumRed += (Gx[m][n] * image[c][d].rgbtRed);
                    ysumRed += (Gy[m][n] * image[c][d].rgbtRed);

                    xsumGreen += (Gx[m][n] * image[c][d].rgbtGreen);
                    ysumGreen += (Gy[m][n] * image[c][d].rgbtGreen);

                    xsumBlue += (Gx[m][n] * image[c][d].rgbtBlue);
                    ysumBlue += (Gy[m][n] * image[c][d].rgbtBlue);

                }
            }


            sumSqrdR = pow(xsumRed, 2) + pow(ysumRed, 2);
            roundsqrtR = round(sqrt(sumSqrdR));
            if (roundsqrtR > 255) // Cap the value at 255
            {
                imgedges[i][j].rgbtRed = 255;
            }
            else
            {
                imgedges[i][j].rgbtRed = roundsqrtR;
            }

            sumSqrdG = pow(xsumGreen, 2) + pow(ysumGreen, 2);
            roundsqrtG = round(sqrt(sumSqrdG));
            if (roundsqrtG > 255) // Cap the value at 255
            {
                imgedges[i][j].rgbtGreen = 255;
            }
            else
            {
                imgedges[i][j].rgbtGreen = roundsqrtG;
            }

            sumSqrdB = pow(xsumBlue, 2) + pow(ysumBlue, 2);
            roundsqrtB = round(sqrt(sumSqrdB));
            if (roundsqrtB > 255) // Cap the value at 255
            {
                imgedges[i][j].rgbtBlue = 255;
            }
            else
            {
                imgedges[i][j].rgbtBlue = roundsqrtB;
            }
        }
    }
    // Overwrite the image with the calculated values.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = imgedges[i][j].rgbtRed;
            image[i][j].rgbtGreen = imgedges[i][j].rgbtGreen;
            image[i][j].rgbtBlue = imgedges[i][j].rgbtBlue;
        }
    }
    return;
}
