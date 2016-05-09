// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "random.h"

uint64_t* random_series(size_t size, uint64_t base)
{
    uint64_t *series = calloc(size, sizeof(uint64_t));
    if (series != NULL)
    {
        for (size_t i = 0; i < size; ++i)
        {
            series[i] = rand() % base;
        }
    }
    return series;
}
