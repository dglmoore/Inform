// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/dist.h>
#include <math.h>

typedef double entropy;

entropy inform_shannon(inform_dist const *dist);

entropy inform_mutual_info(inform_dist const *joint,
        inform_dist const *marginal_x,
        inform_dist const *marginal_y);
