// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdlib.h>
#include <inform/utilities/partitions.h>

size_t *inform_first_partitioning(size_t size)
{
    return (size > 0) ? calloc(size, sizeof(size_t)) : NULL;
}
