// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>

#include <stdio.h>

uint64_t inform_encode(int *state, size_t n)
{
    if (state == NULL)
    {
        return INFORM_ENCODING_ERROR(0);
    }
    else if (n >= 64)
    {
        return INFORM_ENCODING_ERROR(1);
    }
    uint64_t encoding = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (state[i] != 0 && state[i] != 1)
        {
            return INFORM_ENCODING_ERROR(2+i);
        }
        encoding += state[i] * ((uint64_t)1 << i);
    }
    return encoding;
}

int* inform_decode(uint64_t encoding, size_t n)
{
    if (encoding >= inform_encoding_error || n >= 64)
    {
        return NULL;
    }
    int *state = calloc(n, sizeof(int));
    if (state != NULL)
    {
        for (size_t i = 0; i < n; ++i, encoding >>= 1)
        {
            state[i] = encoding & 1; 
        }
    }
    return state;
}
