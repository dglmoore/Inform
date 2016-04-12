// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>

#include <inform/state_encoding.h>
#include <stdio.h>

CTEST(StateEncoding, EncodeNULL)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode(NULL,0));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode(NULL,1));
}

CTEST(StateEncoding, EncodeTooLong)
{
    int *ones = calloc(63, sizeof(int));
    for (size_t i = 0; i < 63; ++i) ones[i] = 1;
    ASSERT_EQUAL_U(0x7fffffffffffffff, inform_encode(ones, 63));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode(ones, 64));
    free(ones);
}

CTEST(StateEncoding, EncodeLengthOne)
{
   ASSERT_EQUAL_U(0, inform_encode((int[]){0}, 1)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1}, 1)); 

   ASSERT_EQUAL_U(0, inform_encode((int[]){0,0}, 1)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,0}, 1)); 
   ASSERT_EQUAL_U(0, inform_encode((int[]){0,1}, 1)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,1}, 1)); 
}

CTEST(StateEncoding, EncodeLengthTwo)
{
   ASSERT_EQUAL_U(0, inform_encode((int[]){0,0}, 2)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,0}, 2)); 
   ASSERT_EQUAL_U(2, inform_encode((int[]){0,1}, 2)); 
   ASSERT_EQUAL_U(3, inform_encode((int[]){1,1}, 2)); 

   ASSERT_EQUAL_U(0, inform_encode((int[]){0,0,0}, 2)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,0,0}, 2)); 
   ASSERT_EQUAL_U(2, inform_encode((int[]){0,1,0}, 2)); 
   ASSERT_EQUAL_U(3, inform_encode((int[]){1,1,0}, 2)); 
   ASSERT_EQUAL_U(0, inform_encode((int[]){0,0,1}, 2)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,0,1}, 2)); 
   ASSERT_EQUAL_U(2, inform_encode((int[]){0,1,1}, 2)); 
   ASSERT_EQUAL_U(3, inform_encode((int[]){1,1,1}, 2)); 
}

CTEST(StateEncoding, EncodeLengthThree)
{
   ASSERT_EQUAL_U(0, inform_encode((int[]){0,0,0}, 3)); 
   ASSERT_EQUAL_U(1, inform_encode((int[]){1,0,0}, 3)); 
   ASSERT_EQUAL_U(2, inform_encode((int[]){0,1,0}, 3)); 
   ASSERT_EQUAL_U(3, inform_encode((int[]){1,1,0}, 3)); 
   ASSERT_EQUAL_U(4, inform_encode((int[]){0,0,1}, 3)); 
   ASSERT_EQUAL_U(5, inform_encode((int[]){1,0,1}, 3)); 
   ASSERT_EQUAL_U(6, inform_encode((int[]){0,1,1}, 3)); 
   ASSERT_EQUAL_U(7, inform_encode((int[]){1,1,1}, 3)); 
}

CTEST(StateEncoding, EncodingNonBoolean)
{
    ASSERT_EQUAL_U(0, inform_encode((int[]){0}, 1));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode((int[]){-1}, 1));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode((int[]){2}, 1));
}

CTEST(StateEncoding, EncodeInvertsDecode)
{
    uint64_t const M = 10;
    for (uint64_t x = 0; x < (uint64_t)1 << M; ++x)
    {
       int *state = inform_decode(x, M); 
       ASSERT_EQUAL_U(x, inform_encode(state, M));
       free(state);
    }
}

CTEST(StateEncoding, DecodeErrors)
{
    ASSERT_NULL(inform_decode(inform_encoding_error, 1));
    ASSERT_NULL(inform_decode(inform_encoding_error + 1, 1));
    ASSERT_NULL(inform_decode(inform_encoding_error + 2, 1));
}

CTEST(StateEncoding, DecodeTooManyBits)
{
    int *state = inform_decode(0,63);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode(0,64));
}
