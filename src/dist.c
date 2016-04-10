// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/dist.h>

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
    inform_dist* dist = malloc(sizeof(inform_dist));
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

void inform_dist_free(inform_dist* dist)
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

size_t inform_dist_size(inform_dist* dist)
{
    return (dist == NULL) ? 0 : dist->size;
}

uint64_t inform_dist_counts(inform_dist* dist)
{
    return (dist == NULL) ? 0 : dist->counts;
}

bool inform_dist_is_valid(inform_dist* dist)
{
    return dist != NULL && dist->size != 0 && dist->counts != 0;
}

uint64_t inform_dist_get(inform_dist* dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    return dist->histogram[event];
}

uint64_t inform_dist_set(inform_dist* dist, uint64_t event, uint64_t x)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    dist->counts -= dist->histogram[event];
    dist->counts += x;
    return (dist->histogram[event] = x);
}

uint64_t inform_dist_tick(inform_dist* dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    dist->counts += 1;
    return (dist->histogram[event] += 1);
}

double inform_dist_prob(inform_dist* dist, uint64_t event)
{
    if (dist == NULL || event >= dist->size)
    {
        return 0;
    }
    return (double)(dist->histogram[event]) / dist->counts;
}
