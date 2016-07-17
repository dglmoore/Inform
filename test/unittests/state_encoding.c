// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>

#include <inform/state_encoding.h>
#include <math.h>

UNIT(EncodeNULL)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode(NULL,0,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode(NULL,1,2));
}

UNIT(EncodeInvalidBase)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode((uint64_t[]){0},1, 0));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode((uint64_t[]){0},1, 1));
}

UNIT(EncodeTooLong_Base2)
{
    uint64_t *ones = calloc(63, sizeof(uint64_t));
    for (size_t i = 0; i < 63; ++i) ones[i] = 1;
    ASSERT_EQUAL_U(0x7fffffffffffffff, inform_encode(ones,63,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode(ones,64,2));
    free(ones);
}

UNIT(EncodeTooLong_Base3)
{
    uint64_t *twos = calloc(40, sizeof(uint64_t));
    for (size_t i = 0; i < 40; ++i) twos[i] = 2;
    ASSERT_EQUAL_U(0xa8b8b452291fe820, inform_encode(twos,40,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode(twos,41,3));
    free(twos);
}

UNIT(EncodeTooLong_Base4)
{
    uint64_t *threes = calloc(31, sizeof(uint64_t));
    for (size_t i = 0; i < 31; ++i) threes[i] = 3;
    ASSERT_EQUAL_U(0x3fffffffffffffff, inform_encode(threes,31,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode(threes,32,4));
    free(threes);
}

UNIT(EncodeTooLong_Base5)
{
    uint64_t *fours = calloc(27, sizeof(uint64_t));
    for (size_t i = 0; i < 27; ++i) fours[i] = 4;
    ASSERT_EQUAL_U(0x6765c793fa10079c, inform_encode(fours,27,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode(fours,28,5));
    free(fours);
}

UNIT(EncodeLengthOne_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0},1,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){1},1,2));

   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,0},1,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){1,0},1,2));
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,1},1,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){1,1},1,2));
}

UNIT(EncodeLengthOne_Base5)
{
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0},1,5));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){1},1,5));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){2},1,5));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){3},1,5));
   ASSERT_EQUAL_U(4, inform_encode((uint64_t[]){4},1,5));

   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,1},1,5));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){1,1},1,5));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){2,1},1,5));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){3,1},1,5));
   ASSERT_EQUAL_U(4, inform_encode((uint64_t[]){4,1},1,5));
}

UNIT(EncodeLengthTwo_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,0},2,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){0,1},2,2));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){1,0},2,2));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){1,1},2,2));

   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,0,0},2,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){0,1,0},2,2));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){1,0,0},2,2));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){1,1,0},2,2));
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,0,1},2,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){0,1,1},2,2));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){1,0,1},2,2));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){1,1,1},2,2));

}

UNIT(EncodeLengthTwo_Base5)
{
   ASSERT_EQUAL_U( 0, inform_encode((uint64_t[]){0,0},2,5));
   ASSERT_EQUAL_U( 1, inform_encode((uint64_t[]){0,1},2,5));
   ASSERT_EQUAL_U( 2, inform_encode((uint64_t[]){0,2},2,5));
   ASSERT_EQUAL_U( 3, inform_encode((uint64_t[]){0,3},2,5));
   ASSERT_EQUAL_U( 4, inform_encode((uint64_t[]){0,4},2,5));
   ASSERT_EQUAL_U( 5, inform_encode((uint64_t[]){1,0},2,5));
   ASSERT_EQUAL_U( 6, inform_encode((uint64_t[]){1,1},2,5));
   ASSERT_EQUAL_U( 7, inform_encode((uint64_t[]){1,2},2,5));
   ASSERT_EQUAL_U( 8, inform_encode((uint64_t[]){1,3},2,5));
   ASSERT_EQUAL_U( 9, inform_encode((uint64_t[]){1,4},2,5));
   ASSERT_EQUAL_U(10, inform_encode((uint64_t[]){2,0},2,5));
   ASSERT_EQUAL_U(11, inform_encode((uint64_t[]){2,1},2,5));
   ASSERT_EQUAL_U(12, inform_encode((uint64_t[]){2,2},2,5));
   ASSERT_EQUAL_U(13, inform_encode((uint64_t[]){2,3},2,5));
   ASSERT_EQUAL_U(14, inform_encode((uint64_t[]){2,4},2,5));
   ASSERT_EQUAL_U(15, inform_encode((uint64_t[]){3,0},2,5));
   ASSERT_EQUAL_U(16, inform_encode((uint64_t[]){3,1},2,5));
   ASSERT_EQUAL_U(17, inform_encode((uint64_t[]){3,2},2,5));
   ASSERT_EQUAL_U(18, inform_encode((uint64_t[]){3,3},2,5));
   ASSERT_EQUAL_U(19, inform_encode((uint64_t[]){3,4},2,5));
   ASSERT_EQUAL_U(20, inform_encode((uint64_t[]){4,0},2,5));
   ASSERT_EQUAL_U(21, inform_encode((uint64_t[]){4,1},2,5));
   ASSERT_EQUAL_U(22, inform_encode((uint64_t[]){4,2},2,5));
   ASSERT_EQUAL_U(23, inform_encode((uint64_t[]){4,3},2,5));
   ASSERT_EQUAL_U(24, inform_encode((uint64_t[]){4,4},2,5));
}

UNIT(EncodeLengthThree_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode((uint64_t[]){0,0,0},3,2));
   ASSERT_EQUAL_U(1, inform_encode((uint64_t[]){0,0,1},3,2));
   ASSERT_EQUAL_U(2, inform_encode((uint64_t[]){0,1,0},3,2));
   ASSERT_EQUAL_U(3, inform_encode((uint64_t[]){0,1,1},3,2));
   ASSERT_EQUAL_U(4, inform_encode((uint64_t[]){1,0,0},3,2));
   ASSERT_EQUAL_U(5, inform_encode((uint64_t[]){1,0,1},3,2));
   ASSERT_EQUAL_U(6, inform_encode((uint64_t[]){1,1,0},3,2));
   ASSERT_EQUAL_U(7, inform_encode((uint64_t[]){1,1,1},3,2));
}

UNIT(EncodeLengthThree_Base3)
{
   ASSERT_EQUAL_U( 0, inform_encode((uint64_t[]){0,0,0},3,3));
   ASSERT_EQUAL_U( 1, inform_encode((uint64_t[]){0,0,1},3,3));
   ASSERT_EQUAL_U( 2, inform_encode((uint64_t[]){0,0,2},3,3));
   ASSERT_EQUAL_U( 3, inform_encode((uint64_t[]){0,1,0},3,3));
   ASSERT_EQUAL_U( 4, inform_encode((uint64_t[]){0,1,1},3,3));
   ASSERT_EQUAL_U( 5, inform_encode((uint64_t[]){0,1,2},3,3));
   ASSERT_EQUAL_U( 6, inform_encode((uint64_t[]){0,2,0},3,3));
   ASSERT_EQUAL_U( 7, inform_encode((uint64_t[]){0,2,1},3,3));
   ASSERT_EQUAL_U( 8, inform_encode((uint64_t[]){0,2,2},3,3));
   ASSERT_EQUAL_U( 9, inform_encode((uint64_t[]){1,0,0},3,3));
   ASSERT_EQUAL_U(10, inform_encode((uint64_t[]){1,0,1},3,3));
   ASSERT_EQUAL_U(11, inform_encode((uint64_t[]){1,0,2},3,3));
   ASSERT_EQUAL_U(12, inform_encode((uint64_t[]){1,1,0},3,3));
   ASSERT_EQUAL_U(13, inform_encode((uint64_t[]){1,1,1},3,3));
   ASSERT_EQUAL_U(14, inform_encode((uint64_t[]){1,1,2},3,3));
   ASSERT_EQUAL_U(15, inform_encode((uint64_t[]){1,2,0},3,3));
   ASSERT_EQUAL_U(16, inform_encode((uint64_t[]){1,2,1},3,3));
   ASSERT_EQUAL_U(17, inform_encode((uint64_t[]){1,2,2},3,3));
   ASSERT_EQUAL_U(18, inform_encode((uint64_t[]){2,0,0},3,3));
   ASSERT_EQUAL_U(19, inform_encode((uint64_t[]){2,0,1},3,3));
   ASSERT_EQUAL_U(20, inform_encode((uint64_t[]){2,0,2},3,3));
   ASSERT_EQUAL_U(21, inform_encode((uint64_t[]){2,1,0},3,3));
   ASSERT_EQUAL_U(22, inform_encode((uint64_t[]){2,1,1},3,3));
   ASSERT_EQUAL_U(23, inform_encode((uint64_t[]){2,1,2},3,3));
   ASSERT_EQUAL_U(24, inform_encode((uint64_t[]){2,2,0},3,3));
   ASSERT_EQUAL_U(25, inform_encode((uint64_t[]){2,2,1},3,3));
   ASSERT_EQUAL_U(26, inform_encode((uint64_t[]){2,2,2},3,3));
}

UNIT(EncodingInvalidState_Base2)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){-1},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){2},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){3},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,2},2,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,3},2,2));
}

UNIT(EncodingInvalidState_Base3)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){-1},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){3},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){4},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,3},2,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,4},2,3));
}

UNIT(EncodingInvalidState_Base4)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){-1},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){4},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){5},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,4},2,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,5},2,4));
}

UNIT(EncodingInvalidState_Base5)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){-1},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){5},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode((uint64_t[]){6},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,5},2,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode((uint64_t[]){0,6},2,5));
}

UNIT(EncodeInvertsDecode_Base2)
{
    int const base = 2;
    size_t const M = 10;
    for (uint64_t x = 0; x < (uint64_t) pow((double) base, (double) M); ++x)
    {
       uint64_t *state = inform_decode(x, M, base);
       ASSERT_EQUAL_U(x, inform_encode(state, M, base));
       free(state);
    }
}

UNIT(EncodeInvertsDecode_Base3)
{
    int const base = 3;
    size_t const M = 6;
    for (uint64_t x = 0; x < (uint64_t) pow((double) base, (double) M); ++x)
    {
       uint64_t *state = inform_decode(x, M, base);
       ASSERT_EQUAL_U(x, inform_encode(state, M, base));
       free(state);
    }
}

UNIT(EncodeInvertsDecode_Base4)
{
    int const base = 4;
    size_t const M = 5;
    for (uint64_t x = 0; x < (uint64_t) pow((double) base, (double) M); ++x)
    {
       uint64_t *state = inform_decode(x, M, base);
       ASSERT_EQUAL_U(x, inform_encode(state, M, base));
       free(state);
    }
}

UNIT(EncodeInvertsDecode_Base5)
{
    int const base = 5;
    size_t const M = 4;
    for (int x = 0; x < (int) pow((double) base, (double) M); ++x)
    {
       uint64_t *state = inform_decode(x, M, base);
       ASSERT_EQUAL_U(x, inform_encode(state, M, base));
       free(state);
    }
}

UNIT(DecodeErrors)
{
    ASSERT_NULL(inform_decode(inform_encoding_error,1,2));
    ASSERT_NULL(inform_decode(inform_encoding_error + 1,1,2));
    ASSERT_NULL(inform_decode(inform_encoding_error + 2,1,2));

    ASSERT_NULL(inform_decode(inform_encoding_error,1,3));
    ASSERT_NULL(inform_decode(inform_encoding_error + 1,1,3));
    ASSERT_NULL(inform_decode(inform_encoding_error + 2,1,3));
}

UNIT(DecodeTooManyBits_Base2)
{
    uint64_t *state = inform_decode(0,63,2);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode(0,64,2));
}

UNIT(DecodeTooManyBits_Base3)
{
    uint64_t *state = inform_decode(0,40,3);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode(0,41,3));
}

UNIT(DecodeTooManyBits_Base4)
{
    uint64_t *state = inform_decode(0,31,4);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode(0,32,4));
}

UNIT(DecodeTooManyBits_Base5)
{
    uint64_t *state = inform_decode(0,27,5);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode(0,28,5));
}

BEGIN_SUITE(StateEncoding)
    ADD_UNIT(EncodeNULL)
    ADD_UNIT(EncodeInvalidBase)
    ADD_UNIT(EncodeTooLong_Base2)
    ADD_UNIT(EncodeTooLong_Base3)
    ADD_UNIT(EncodeTooLong_Base4)
    ADD_UNIT(EncodeTooLong_Base5)
    ADD_UNIT(EncodeLengthOne_Base2)
    ADD_UNIT(EncodeLengthOne_Base5)
    ADD_UNIT(EncodeLengthTwo_Base2)
    ADD_UNIT(EncodeLengthTwo_Base5)
    ADD_UNIT(EncodeLengthThree_Base2)
    ADD_UNIT(EncodeLengthThree_Base3)
    ADD_UNIT(EncodingInvalidState_Base2)
    ADD_UNIT(EncodingInvalidState_Base3)
    ADD_UNIT(EncodingInvalidState_Base4)
    ADD_UNIT(EncodingInvalidState_Base5)
    ADD_UNIT(EncodeInvertsDecode_Base2)
    ADD_UNIT(EncodeInvertsDecode_Base3)
    ADD_UNIT(EncodeInvertsDecode_Base4)
    ADD_UNIT(EncodeInvertsDecode_Base5)
    ADD_UNIT(DecodeErrors)
    ADD_UNIT(DecodeTooManyBits_Base2)
    ADD_UNIT(DecodeTooManyBits_Base3)
    ADD_UNIT(DecodeTooManyBits_Base4)
    ADD_UNIT(DecodeTooManyBits_Base5)
END_SUITE
