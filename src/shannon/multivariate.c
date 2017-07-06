// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/shannon/multivariate.h>
#include <inform/shannon/univariate.h>
#include <inform/error.h>

double inform_shannon_multi_pmi(inform_dist const *joint,
    inform_dist const **marginals, size_t n, size_t joint_event,
    size_t const *marginal_events, double base)
{
    if (n < 2)
    {
        return 0.0;
    }
    double pmi = -inform_shannon_si(joint, joint_event, base);
    for (size_t i = 0; i < n; ++i)
    {
        pmi += inform_shannon_si(marginals[i], marginal_events[i], base);
    }
    return pmi;
}

double inform_shannon_multi_mi(inform_dist const *joint,
    inform_dist const **marginals, size_t n, double base)
{
    if (n < 2)
    {
        return 0.0;
    }
    double pmi = -inform_shannon_entropy(joint, base);
    for (size_t i = 0; i < n; ++i)
    {
        pmi += inform_shannon_entropy(marginals[i], base);
    }
    return pmi;
}
