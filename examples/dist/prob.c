#include <assert.h>
#include <inform/dist.h>
#include <math.h>

int main()
{
    inform_dist *dist = inform_dist_create((uint32_t[4]){3, 0, 1, 2}, 4);
    assert(abs(inform_dist_prob(dist, 0) -    0.5) < 1e-6);
    assert(abs(inform_dist_prob(dist, 1) -    0.0) < 1e-6);
    assert(abs(inform_dist_prob(dist, 2) - 0.1666) < 1e-6);
    assert(abs(inform_dist_prob(dist, 3) - 0.3333) < 1e-6);
    inform_dist_free(dist);
}
