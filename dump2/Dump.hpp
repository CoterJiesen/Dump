
#ifndef _DUMP_H
#define _DUMP_H

#include <dbghelp.h> 
#include <shellapi.h> 
#include <shlobj.h> 

#pragma comment(lib,"dbghelp.lib")  
  
// 自定义的exectpion filter 

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers) 
{ 
  
    SetErrorMode( SEM_NOGPFAULTERRORBOX ); 
  
	TCHAR strBuild[MAX_PATH] = {0}; 
   // strBuild.Format(L"Build: %s %s", __DATE__, __TIME__); 
	_stprintf_s(strBuild,_T("Build: %s %s"), __DATE__, __TIME__);
    TCHAR strError[MAX_PATH]; 
    HMODULE hModule; 
	TCHAR szModuleName[MAX_PATH] = {0}; 
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)pExceptionPointers->ExceptionRecord->ExceptionAddress, &hModule); 
    GetModuleFileName(hModule, szModuleName, ARRAYSIZE(szModuleName)); 
    _stprintf_s(strError, _T("%s %d , %d ,%d."), 
				szModuleName,
				pExceptionPointers->ExceptionRecord->ExceptionCode, 
				pExceptionPointers->ExceptionRecord->ExceptionFlags,
				pExceptionPointers->ExceptionRecord->ExceptionAddress); 
  
    //生成 mini crash dump 

    BOOL bMiniDumpSuccessful; 
    WCHAR szPath[MAX_PATH]; 
    WCHAR szFileName[MAX_PATH]; 
    WCHAR* szAppName = L"AppName"; 
    WCHAR* szVersion = L"v1.0"; 
    DWORD dwBufferSize = MAX_PATH; 
    HANDLE hDumpFile; 
    SYSTEMTIME stLocalTime; 
    MINIDUMP_EXCEPTION_INFORMATION ExpParam; 
    GetLocalTime( &stLocalTime ); 
    GetTempPath( dwBufferSize, szPath ); 
    _stprintf_s( szFileName, MAX_PATH, L"%s%s", szPath, szAppName ); 
    CreateDirectory( szFileName, NULL ); 
    _stprintf_s( szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
               szPath, szAppName, szVersion, 
               stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
               stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
               GetCurrentProcessId(), GetCurrentThreadId()); 
    hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
                FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0); 
  
    MINIDUMP_USER_STREAM UserStream[2]; 
    MINIDUMP_USER_STREAM_INFORMATION UserInfo; 
    UserInfo.UserStreamCount = 1; 
    UserInfo.UserStreamArray = UserStream; 
    UserStream[0].Type = CommentStreamW; 
    UserStream[0].BufferSize = _tcslen(strBuild) *sizeof(TCHAR); 
    UserStream[0].Buffer = strBuild; 
    UserStream[1].Type = CommentStreamW; 
    UserStream[1].BufferSize = _tcslen(strError)*sizeof(TCHAR); 
    UserStream[1].Buffer = strError;
  
    ExpParam.ThreadId = GetCurrentThreadId(); 
    ExpParam.ExceptionPointers = pExceptionPointers; 
    ExpParam.ClientPointers = TRUE; 
       
    MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal 
										 | MiniDumpWithHandleData 
										 | MiniDumpWithUnloadedModules 
										 | MiniDumpWithIndirectlyReferencedMemory 
										 | MiniDumpScanMemory 
										 | MiniDumpWithProcessThreadData 
										 | MiniDumpWithThreadInfo); 

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                    hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL); 

  
  return EXCEPTION_CONTINUE_SEARCH; 

} 
   
#endif

 /*
int _tmain() 
{ 
 

  SetUnhandledExceptionFilter(MyUnhandledExceptionFilter); 
 
  return 0; 
} 
*/
