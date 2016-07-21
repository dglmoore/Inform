// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "util.h"

int* random_series(size_t size, int base)
{
    int *series = calloc(size, sizeof(int));
    if (series != NULL)
    {
        for (size_t i = 0; i < size; ++i)
        {
            series[i] = rand() % base;
        }
    }
    return series;
}

double average(double const *xs, size_t n)
{
    double x = 0;
    for (size_t i = 0; i < n; ++i)
    {
        x += xs[i];
    }
    return x / n;
}
