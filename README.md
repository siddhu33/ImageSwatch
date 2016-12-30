# Image Swatch
## Before
![before processing](https://upload.wikimedia.org/wikipedia/commons/e/e6/Big.Buck.Bunny.-.Landscape.png)
## After
![after processing](http://i.imgur.com/cyRsf0w.png)

## Description

This program is designed to create a swatch (sample) out of any image, providing a selection of colours that can be clearly seen within it. This helps for when you want to design something based on a particular image; giving you a selection of colours that you can then find complements of, or use for use when attempting to capture the same mood that the colours within a particular image provides.

## Installation

Swatch currently works on any C++ compiler that has C++11 and an installation of libpng (for manipulating PNG images).

### Linux / Mac

Once libpng has been added, run the makefile, which will compile the application. Make clean will remove all files.
```
make #compile from source
make clean #clean directory.
```

### Windows

I have not provided an .exe for Windows, but to compile this on windows, all that is needed is to ensure that libpng is correctly
added to both compilation and linking steps, and ensuring the libpng dll files are correctly placed where the program is executed.

```
g++ [COMPILE ARGS] [SOURCE FILES] \
-I [libpng includes] -L [libpng .lib files] -o swatch -lpng
```


## Further Work

This is that fun stuff that pull requests help with:

1. Support for importing .jpg files to the script.
2. Better command line parsing for further functionality (number of top colours, size of output image, vertical swatch, etc).
3. Look into the Lightness-A-B color spectrum instead of Hue-Saturation-Value for color segmentation.
4. Parallelize the color reduction step using either multi-threading or GPUs.