# Obfuscation Integration Guide

This guide explains how to use obfusheader.h with the target project for nocrt builds using mingw64.

## Overview

The Makefile now supports obfuscated builds with the following features:
- **NoCRT (No C Runtime Library)**: Reduces binary size and dependencies
- **obfusheader.h Integration**: Compile-time obfuscation for strings, constants, and control flow
- **mingw64 Support**: Cross-compilation for Windows x64/x86 targets

## Build Targets

### Regular WebSocket Build
```bash
make ws
```
Builds WebSocket beacon with standard CRT and no obfuscation.

### Obfuscated WebSocket Build (NoCRT)
```bash
make obfus_ws
```
Builds WebSocket beacon with:
- NoCRT (no standard library dependencies)
- obfusheader.h obfuscation enabled
- mingw64 toolchain

## Obfuscation Features

### Enabled Features
- `CONST_ENCRYPTION=1`: Compile-time string encryption
- `CFLOW_BRANCHING=1`: Control flow obfuscation
- `INDIRECT_BRANCHING=1`: Indirect function calls
- `FAKE_SIGNATURES=1`: Fake PE signatures for anti-analysis

### Available Macros

#### String Obfuscation
```cpp
// Instead of: const char* name = "MySecretString";
const char* name = OBFUS("MySecretString");
```

#### Constant Obfuscation
```cpp
// Instead of: const int port = 443;
const int port = OBFUS(443);
```

#### Function Call Obfuscation
```cpp
// Instead of: WSAStartup(&MAKEWORD(2, 2), &wsaData);
HIDDEN_CALL(WSAStartup)(&MAKEWORD(2, 2), &wsaData);
```

#### Control Flow Obfuscation
```cpp
BLOCK_TRUE({
    // This code block will be obfuscated
    socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL);
});

// Indirect branching to break disassembly
INDIRECT_BRANCH;
```

## Integration in Source Code

### Method 1: Direct Include
Add to the top of your source files:
```cpp
#include "../include/obfusheader.h"
```

### Method 2: Wrapper Header (Recommended)
Create a wrapper header like `obfuscation.h`:
```cpp
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/obfusheader.h"

// Convenience macros
#define OBFUS_STR(str) OBFUS(str)
#define OBFUS_FUNC(func) OBFUSFUNC(func)
#define HIDDEN_CALL(func) CALL_EXPORT(func)

#ifdef __cplusplus
}
#endif
```

Then include in your source:
```cpp
#include "obfuscation.h"
```

## NoCRT Configuration

The obfuscated build uses these NoCRT flags:
- `-nostdlib`: Don't link standard library
- `-nodefaultlibs`: No default libraries
- `-fno-exceptions`: Disable C++ exceptions
- `-fno-unwind-tables`: No unwind tables
- `-DKERNEL_MODE=1`: Kernel mode compatibility

## Compiler Flags

### Common Flags
```makefile
COMMON_FLAGS := -I $(BEACON_DIR) \
               -I ../include \
               -fpermissive \
               -w \
               -masm=intel \
               -fPIC \
               $(SECURITY_FLAGS) \
               $(OPTIMIZATION_FLAGS)
```

### Obfuscation Flags
```makefile
OBFUSCATION_FLAGS := -DCONST_ENCRYPTION=1 \
                     -DCFLOW_BRANCHING=1 \
                     -DINDIRECT_BRANCHING=1 \
                     -DFAKE_SIGNATURES=1
```

### NoCRT Flags
```makefile
NOCRT_FLAGS := -nostdlib \
               -nodefaultlibs \
               -fno-exceptions \
               -fno-unwind-tables \
               -fno-asynchronous-unwind-tables \
               -DKERNEL_MODE=1 \
               -DINLINE_STD=0
```

## Example Usage

See `obfuscated_example.h` for a complete example of how to integrate obfuscation into your classes and functions.

## Benefits

1. **Smaller Binary Size**: NoCRT reduces dependencies
2. **Anti-Analysis**: Obfuscation hinders reverse engineering
3. **Cross-Platform**: mingw64 enables Linux development
4. **Compile-Time**: No runtime overhead for obfuscation

## Notes

- obfusheader.h works with both C and C++
- mingw64 provides excellent Windows compatibility
- NoCRT builds may require manual implementation of some functions
- Test thoroughly after enabling obfuscation

## Troubleshooting

If you encounter compilation errors:
1. Ensure obfusheader.h is in the include path
2. Check for missing function implementations (NoCRT)
3. Verify macro syntax matches obfusheader.h documentation
4. Test without obfuscation first, then enable gradually