// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/entropy.h>

entropy inform_shannon(inform_dist const *dist)
{
    if (inform_dist_is_valid(dist))
    {
        entropy h = 0.;
        for (size_t i = 0; i < inform_dist_size(dist); ++i)
        {
            double const p = inform_dist_prob(dist, i);
            if (p != 0)
            {
                h -= p * log2(p);
            }
        }
        return h;
    }
    return nan("1");
}

entropy inform_mutual_info(inform_dist const *joint,
        inform_dist const *marginal_x, inform_dist const *marginal_y)
{
    return inform_shannon(marginal_x) + inform_shannon(marginal_y)
        - inform_shannon(joint);
}
