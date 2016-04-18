// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>

#include <math.h>
#include <stdio.h>

uint64_t inform_encode_base(int const *state, size_t n, int base)
{
    if (state == NULL)
    {
        return INFORM_ENCODING_ERROR(0);
    }
    else if (base < 2)
    {
        return INFORM_ENCODING_ERROR(1);
    }
    else if (n >= 64 / log2l(base))
    {
        return INFORM_ENCODING_ERROR(2);
    }
    uint64_t encoding = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (base <= state[i] || state[i] < 0)
        {
            return INFORM_ENCODING_ERROR(3+i);
        }
        encoding += state[i] * (uint64_t)powl(base,i);
    }
    return encoding;
}

int* inform_decode_base(uint64_t encoding, size_t n, int base)
{
    if (encoding >= inform_encoding_error || base < 2 || n >= 64 / log2l(base))
    {
        return NULL;
    }
    int *state = calloc(n, sizeof(int));
    if (state != NULL)
    {
        for (size_t i = 0; i < n; ++i, encoding /= base)
        {
            state[i] = encoding % base;
        }
    }
    return state;
}
