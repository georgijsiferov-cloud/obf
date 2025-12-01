#!/usr/bin/env python3  
# -*- coding:utf-8 -*-  
  
import sys  
  
def djb2a(input_str: str) -> int:  
    input_str = input_str.lower()  
    hash_value =1572  
    for char in input_str:  
        hash_value = ((hash_value << 5) + hash_value) + ord(char)  
    return hash_value & 0xFFFFFFFF  
  
def djb2w(input_bytes: bytes) -> int:  
    input_bytes = input_bytes.lower()  
    hash_value = 1572  
    for i in range(0, len(input_bytes), 2):  
        val = int.from_bytes(input_bytes[i:i+2], 'little')  
        hash_value = ((hash_value << 5) + hash_value) + val  
    return hash_value & 0xFFFFFFFF  
  
##############################################  
  
libs = [  
    'ntdll.dll',  
    'kernel32.dll',  
    'iphlpapi.dll',  
    'advapi32.dll',  
    'msvcrt.dll',  
    'winhttp.dll',  
]  
  
functions = """  
//ntdll  
NtClose  
NtContinue  
NtFreeVirtualMemory  
NtQueryInformationProcess  
NtQuerySystemInformation  
NtOpenProcess  
NtOpenProcessToken  
NtOpenThreadToken  
NtTerminateThread  
NtTerminateProcess  
RtlGetVersion  
RtlExitUserThread  
RtlExitUserProcess  
RtlIpv4StringToAddressA  
RtlRandomEx  
RtlNtStatusToDosError  
NtFlushInstructionCache  
//kernel32  
ConnectNamedPipe  
CopyFileA  
CreateDirectoryA  
CreateFileA  
CreateNamedPipeA  
CreatePipe  
CreateProcessA  
DeleteFileA  
DisconnectNamedPipe  
FindClose  
FindFirstFileA  
FindNextFileA  
FreeLibrary  
FlushFileBuffers  
GetACP  
GetComputerNameExA  
GetCurrentDirectoryA  
GetDriveTypeA  
GetExitCodeProcess  
GetExitCodeThread  
GetFileSize  
GetFileAttributesA  
GetFullPathNameA  
GetLastError  
GetLogicalDrives  
GetOEMCP  
K32GetModuleBaseNameA  
GetModuleBaseNameA  
GetModuleHandleA  
GetProcAddress  
GetTickCount  
GetTimeZoneInformation  
GetUserNameA  
HeapAlloc  
HeapCreate  
HeapDestroy  
HeapReAlloc  
HeapFree  
IsWow64Process  
LoadLibraryA  
LocalAlloc  
LocalFree  
LocalReAlloc  
MoveFileA  
MultiByteToWideChar  
PeekNamedPipe  
ReadFile  
RemoveDirectoryA  
RtlCaptureContext  
SetCurrentDirectoryA  
SetNamedPipeHandleState  
Sleep  
VirtualAlloc  
VirtualFree  
WaitNamedPipeA  
WideCharToMultiByte  
WriteFile  
// iphlpapi  
GetAdaptersInfo  
// advapi32  
AllocateAndInitializeSid  
GetTokenInformation  
InitializeSecurityDescriptor  
ImpersonateLoggedOnUser  
FreeSid  
LookupAccountSidA  
RevertToSelf  
SetThreadToken  
SetEntriesInAclA  
SetSecurityDescriptorDacl  
// msvcrt  
printf  
vsnprintf  
// BOF  
BeaconDataParse  
BeaconDataInt  
BeaconDataShort  
BeaconDataLength  
BeaconDataExtract  
BeaconFormatAlloc  
BeaconFormatReset  
BeaconFormatAppend  
BeaconFormatPrintf  
BeaconFormatToString  
BeaconFormatFree  
BeaconFormatInt  
BeaconOutput  
BeaconPrintf  
BeaconUseToken  
BeaconRevertToken  
BeaconIsAdmin  
BeaconGetSpawnTo  
BeaconInjectProcess  
BeaconInjectTemporaryProcess  
BeaconSpawnTemporaryProcess  
BeaconCleanupProcess  
toWideChar  
BeaconInformation  
BeaconAddValue  
BeaconGetValue  
BeaconRemoveValue  
LoadLibraryA  
GetLocalTime  
GetSystemTimeAsFileTime  
GetProcAddress  
GetModuleHandleA  
FreeLibrary  
__C_specific_handler  
AxAddScreenshot  
AxDownloadMemory  
// wininet  
InternetOpenA  
InternetConnectA  
HttpOpenRequestA  
HttpSendRequestA  
InternetSetOptionA  
InternetQueryOptionA  
HttpQueryInfoA  
InternetQueryDataAvailable  
InternetCloseHandle  
InternetReadFile  
// ws2_32  
WSAStartup  
WSACleanup  
socket  
gethostbyname  
ioctlsocket  
connect  
WSAGetLastError  
closesocket  
select  
__WSAFDIsSet  
shutdown  
recv  
recvfrom  
send  
accept  
bind  
listen  
sendto  
// winhttp  
WinHttpOpen  
WinHttpConnect  
WinHttpOpenRequest  
WinHttpSendRequest  
WinHttpReceiveResponse  
WinHttpQueryOption  
WinHttpQueryHeaders  
WinHttpQueryDataAvailable  
WinHttpReadData  
WinHttpSetOption  
WinHttpAddRequestHeaders  
WinHttpWebSocketCompleteUpgrade  
WinHttpWebSocketSend  
WinHttpWebSocketReceive  
WinHttpWebSocketClose  
WinHttpCloseHandle  
"""  
##############################################  
  
print('#pragma once')  
print()  
  
for lib in libs:  
    lib_bytes = lib.encode('utf-16-le')  
    name = lib.upper().split(".")[0]  
    print('#define HASH_LIB_%s%s0x%x' % (name, (35-len(lib_bytes))*" ", djb2w(lib_bytes)))  
  
print()  
for f in functions.split('\n'):  
    f = f.strip()  # 添加这一行  
    if len(f) == 0:  
        print()  
    elif f[:2]=='//':  
        print(f)  
    else:  
        print('#define HASH_FUNC_%s%s0x%x' % (f.upper(), (35-len(f))*" ", djb2a(f)))
print(hex(djb2a("WinHttpCloseHandle")))  