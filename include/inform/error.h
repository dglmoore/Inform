// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Error tags to be used in conjunction with `inform_error`
 */
typedef enum
{
    INFORM_SUCCESS =  0, /// no error occurred
    INFORM_FAILURE = -1, /// an unspecified error occurred
    INFORM_EFAULT  =  1, /// an invalid pointer was encountered
    INFORM_EINVAL  =  2, /// an invalid agrument was provided
    INFORM_ENOMEM  =  3, /// malloc failed to allocate memory
} inform_error_tag;

/**
 * An error structure for propogating meaningful errors
 */
typedef struct inform_error
{
    int errno;          /// the error number (i.e. an inform_error_tag)
    char *msg;          /// the statically allocated error message
    int line;           /// the line number
    char const *file;   /// the filename
} inform_error;

/// A default success value for errors
#define INFORM_ERROR_SUCCESS (inform_error) {\
    .errno = INFORM_SUCCESS,\
    .msg = NULL,\
    .line = __LINE__,\
    .file = __FILE__,}

/// set an error as pointed to by ERR
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

/// set an error and return a non-void result
#define INFORM_ERROR_RETURN(ERR, ERRNO, MSG, RET) do {\
        INFORM_ERROR(ERR, ERRNO, MSG); \
        return RET; \
    } while(0)

/// set an error and return a void result
#define INFORM_ERROR_RETURN_VOID(ERR, ERRNO, MSG) do {\
        INFORM_ERROR(ERR, ERRNO, MSG); \
        return; \
    } while(0)

/// is the error a "success", i.e. no error
#define INFORM_IS_SUCCESS(ERR) ((ERR) == NULL || (ERR)->errno == INFORM_SUCCESS)

/// is the error a "failure", i.e. not a "success"
#define INFORM_IS_FAILURE(ERR) !INFORM_IS_SUCCESS(ERR)

#ifdef __cplusplus
}
#endif
