#include "stdafx.h"
#include "MCrashDump.h"

// CrashExceptionDump �Լ��� ����Ϸ��� dbghelp.dll�� �ʿ��ϴ�.
// �ش� dll�� http://www.microsoft.com/whdc/ddk/debugging/ ���� ���� �� �ִ�.
#pragma comment(lib, "dbghelp.lib") 


DWORD CrashExceptionDump(PEXCEPTION_POINTERS ExceptionInfo, const char* szDumpFileName, bool bMLog)
{
	// Custom: Force rewrite filename with timestamp for Gunz.dmp
	char szNewFileName[MAX_PATH];
	memset( szNewFileName, 0, sizeof( szNewFileName ) );
	strcpy( szNewFileName, szDumpFileName );

	if( !stricmp( szNewFileName, "Gunz.dmp" ) )
	{
		SYSTEMTIME t;
		GetLocalTime( &t );

		sprintf( szNewFileName, "Gunz_%4d-%02d-%02d_%02d-%02d-%02d.dmp", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}

	HANDLE hFile = CreateFile(
		szNewFileName, // Custom: Force rewrite filename with timestamp for Gunz.dmp
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = ExceptionInfo;
		eInfo.ClientPointers = FALSE;

		MINIDUMP_CALLBACK_INFORMATION cbMiniDump;
		//cbMiniDump.CallbackRoutine = miniDumpCallback;
		cbMiniDump.CallbackRoutine = NULL;
		cbMiniDump.CallbackParam = 0;

		if( MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			ExceptionInfo ? &eInfo : NULL,
			NULL,
			NULL) == false )//&cbMiniDump); )
		{
			
			mlog("Can't Create Dump - MiniDumpWriteDump()\n");
		}		
	}else
	{
		mlog("Can't Create Dump - INVALID_HANDLE_VALUE\n");
	}
	
	CloseHandle(hFile);

	if (bMLog)
	{
		MFilterException(ExceptionInfo);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
