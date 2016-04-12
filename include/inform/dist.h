// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct inform_distribution inform_dist;

inform_dist* inform_dist_alloc(size_t n);
inform_dist* inform_dist_realloc(inform_dist *dist, size_t n);
inform_dist* inform_dist_copy(inform_dist const *source, inform_dist *dest);
inform_dist* inform_dist_dup(inform_dist const *dist);
void inform_dist_free(inform_dist *dist);

size_t inform_dist_size(inform_dist const *dist);
uint64_t inform_dist_counts(inform_dist const *dist);
bool inform_dist_is_valid(inform_dist const *dist);

uint64_t inform_dist_get(inform_dist const *dist, uint64_t event);
uint64_t inform_dist_set(inform_dist *dist, uint64_t event, uint64_t x);

uint64_t inform_dist_tick(inform_dist *dist, uint64_t event);

double inform_dist_prob(inform_dist const *dist, uint64_t event);
double* inform_dist_dump(inform_dist const *dist);
