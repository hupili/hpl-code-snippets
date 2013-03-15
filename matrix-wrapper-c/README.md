# Matrix Wrapper (C)

This is a matrix wrapper for C I wrote in the fall of 3rd undergrdaduate year. 

## Prerequisites

   * `BLAS`
   * `CLAPACK`

## Files

   * `matrix_lib.h`, `matrix_lib.c`: 
   The C wrapper for BLAS and CLAPACK functions. 
   Those functions mimic the interface of MATLAB functions
   so that translation from MATLAB to C will be easier. 
   * `lle.c`: 
   My translation of the Locally Linear Embedding (LLE) from MATLAB to C. 
   It will be good demo for the user of this matrix lib. 

## Test Environment

With certain tuning, it is portable to most major platforms. 
I tested it under:

   * Windows Vista
   * VC6.0
   * Precompiled x86 lib of BLAS and CLAPACK

## Caveats

   * You may want to modify the inclusion of headers in those files for your platform. 
   * To pass library files to your linker, `#pragma` may not always work. 
   It is originally for VC6.0. 
   You may want to pass some compiler functions, like `-l` or `-L`.
   * If you want to use this C port of LLE for production purpose, watch out. 
   It is slow than the MATLAB companion. 
   This is because I'm not traslating the MATLAB's `eigs` function, 
   which can perform a partial computation of eign-decomposition. 
   Since LLE only cares about the several smallest eigen values, 
   the partial computation really matters. 
   If you want to make a production grade translation, 
   check out this 
   [manual page](http://www.netlib.org/lapack/lug/node32.html#1678)
   to find out whether there is a candidate 
   for the MATLAB's `eigs` function. 

## Others 

   * Feel free to modify it for your own research. 
   * Any problems, please raise an issue. 

