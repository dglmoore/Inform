// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>

#include <math.h>
#include <stdio.h>

uint64_t inform_encode(uint64_t const *state, size_t n, int base)
{
    // if the state is NULL, return an error
    if (state == NULL)
    {
        return INFORM_ENCODING_ERROR(0);
    }
    // if the base is less than two, return an error
    else if (base < 2)
    {
        return INFORM_ENCODING_ERROR(1);
    }
    // if the encoding will exceed 63 bits, return an error
    else if (n >= (uint64_t) 64 / log2((double) base))
    {
        return INFORM_ENCODING_ERROR(2);
    }
    
    uint64_t encoding = 0;
    // for each digit
    for (uint64_t i = 0; i < n; ++i)
    {
        // if the digit is too large, return an error
        if (base <= state[i])
        {
            return INFORM_ENCODING_ERROR(3+i);
        }
        // otherwise, encode that value
        encoding *= base;
        encoding += state[i];
    }
    // return the encoding
    return encoding;
}

uint64_t* inform_decode(uint64_t encoding, size_t n, int base)
{
    // If the encoding is invalid, the base is too small, or the requested
    // decoded length is too large, then return NULL.
    if (encoding >= inform_encoding_error || base < 2 || n >= 64 / log2((double) base))
    {
        return NULL;
    }
    // allocate the memory for the decoded state
    uint64_t *state = calloc(n, sizeof(uint64_t));
    // if the allocation succeeds
    if (state != NULL)
    {
        // loop over the digits and extract the decoded value
        for (uint64_t i = 0; i < n; ++i, encoding /= base)
        {
        	state[n-i-1] = encoding % base;
        }
    }
    // return the (possibly NULL) state
    return state;
}
