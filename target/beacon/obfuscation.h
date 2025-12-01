#pragma once

// Enable obfusheader.h for nocrt builds
#ifdef __cplusplus
extern "C" {
#endif

// Include obfusheader.h with appropriate configuration
#include "../include/obfusheader.h"

// Common obfuscation macros for the project
#define OBFUS_STR(str) OBFUS(str)
#define OBFUS_FUNC(func) OBFUSFUNC(func)
#define HIDDEN_CALL(func) CALL_EXPORT(func)

#ifdef __cplusplus
}
#endif