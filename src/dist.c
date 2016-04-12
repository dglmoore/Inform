// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/dist.h>

#include <string.h>

struct inform_distribution
{
    size_t size;
    uint64_t *histogram;
    uint64_t counts;
};

inform_dist* inform_dist_alloc(size_t n)
{
    if (n == 0)
    {
        return NULL;
    }
    inform_dist *dist = malloc(sizeof(inform_dist));
    if (dist != NULL)
    {
        dist->histogram = calloc(n, sizeof(uint64_t));
        if (dist->histogram != NULL)
        {
            dist->size   = n;
            dist->counts = 0;
        }
        else
        {
            free(dist);
        }
    }
    return dist;
}

inform_dist* inform_dist_realloc(inform_dist *dist, size_t n)
{
    if (dist != NULL && dist->size != n)
    {
        uint64_t *histogram = realloc(dist->histogram, n * sizeof(uint64_t));
        if (histogram != NULL)
        {
            dist->histogram = histogram;
            if (n < dist->size)
            {
                dist->size = n;
                dist->counts = 0;
                for (size_t i = 0; i < dist->size; ++i)
                {
                    dist->counts += dist->histogram[i];
                }
            }
            else
            {
                for (size_t i = dist->size; i < n; ++i)
                {
                    dist->histogram[i] = 0;
                }
                dist->size = n;
            }
        }
        else
        {
            return NULL;
        }
    }
    else if (dist == NULL)
    {
        dist = inform_dist_alloc(n);
    }
    return dist;
}

inform_dist* inform_dist_copy(inform_dist const *src, inform_dist *dest)
{
    if (src == NULL)
    {
        return NULL;
    }
    else if (dest == NULL || src->size != dest->size)
    {
        inform_dist *redest = NULL;
        if ((redest = inform_dist_realloc(dest, src->size)) == NULL)
        {
            return NULL;
        }
        dest = redest;
    }
    else if (src == dest)
    {
        return dest;
    }
    memcpy(src->histogram, dest->histogram, src->size * sizeof(uint64_t));
    dest->counts = src->counts;
    return dest;
}

inform_dist* inform_dist_dup(inform_dist const *dist)
{
    if (dist == NULL)
    {
        return NULL;
    }
    inform_dist *dup = inform_dist_alloc(dist->size);
    if (dup != NULL)
    {
        inform_dist_copy(dist, dup);
    }
    return dup;
}

void inform_dist_free(inform_dist *dist)
{
    if (dist != NULL)
    {
        if (dist->histogram != NULL)
        {
            free(dist->histogram);
        }
        free(dist);
    }
}

size_t inform_dist_size(inform_dist const *dist)
{
    return (dist == NULL) ? 0 : dist->size;
}

uint64_t inform_dist_counts(inform_dist const *dist)
{
    return (dist == NULL) ? 0 : dist->counts;
}

bool inform_dist_is_valid(inform_dist const *dist)
{
    return dist != NULL && dist->size != 0 && dist->counts != 0;
}

uint64_t inform_dist_get(inform_dist const *dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    return dist->histogram[event];
}

uint64_t inform_dist_set(inform_dist *dist, uint64_t event, uint64_t x)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    dist->counts -= dist->histogram[event];
    dist->counts += x;
    return (dist->histogram[event] = x);
}

uint64_t inform_dist_tick(inform_dist *dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    dist->counts += 1;
    return (dist->histogram[event] += 1);
}

static double inform_dist_unsafe_prob(inform_dist const *dist, uint64_t event)
{
    return (double)(dist->histogram[event]) / dist->counts;
}

double inform_dist_prob(inform_dist const *dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    return inform_dist_unsafe_prob(dist, event);
}

double* inform_dist_dump(inform_dist const *dist)
{
    if (dist == NULL || dist->size == 0)
    {
        return NULL;
    }
    double *probabilities = calloc(dist->size, sizeof(double));
    if (probabilities != NULL)
    {
        for (size_t i = 0; i < inform_dist_size(dist); ++i)
        {
            probabilities[i] = inform_dist_unsafe_prob(dist,i);
        }
    }
    return probabilities;
}
