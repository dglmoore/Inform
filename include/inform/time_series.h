// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/entropy.h>

entropy inform_active_info(int const *series, size_t n, int base, uint64_t k);

entropy inform_active_info_ensemble(int const *series, size_t m, size_t n, int base, uint64_t k);
