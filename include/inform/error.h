// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    INFORM_SUCCESS =  0,
    INFORM_FAILURE = -1,
    INFORM_EFAULT  =  1,
    INFORM_EINVAL  =  2,
    INFORM_ENOMEM  =  3,
} inform_error_tag;

typedef struct inform_error
{
    int errno;
    char *msg;
    int line;
    char const *file;
} inform_error;

#define INFORM_ERROR_SUCCESS (inform_error) {\
    .errno = INFORM_SUCCESS,\
    .msg = NULL,\
    .line = __LINE__,\
    .file = __FILE__,}

#define INFORM_ERROR(ERR, ERRNO, MSG) do {\
        if ((ERR) != NULL) {\
            *(ERR) = (inform_error) {\
                .errno = (ERRNO),\
                .msg = (MSG),\
                .line = __LINE__,\
                .file = __FILE__,\
            };\
        }\
    } while(0)

#define INFORM_ERROR_RETURN(ERR, ERRNO, MSG, RET) do {\
        INFORM_ERROR(ERR, ERRNO, MSG); \
        return RET; \
    } while(0)

#define INFORM_ERROR_RETURN_VOID(ERR, ERRNO, MSG) do {\
        INFORM_ERROR(ERR, ERRNO, MSG); \
        return; \
    } while(0)

#define INFORM_IS_SUCCESS(ERR) ((ERR) == NULL || (ERR)->errno == INFORM_SUCCESS)
#define INFORM_IS_FAILURE(ERR) !INFORM_IS_SUCCESS(ERR)

#ifdef __cplusplus
}
#endif
