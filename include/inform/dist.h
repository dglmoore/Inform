// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct inform_distribution inform_dist;

inform_dist* inform_dist_alloc(size_t n);
void inform_dist_free(inform_dist* dist);

size_t inform_dist_size(inform_dist* dist);
uint64_t inform_dist_counts(inform_dist* dist);
bool inform_dist_is_valid(inform_dist* dist);

