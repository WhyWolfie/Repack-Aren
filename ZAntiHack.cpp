#include "StdAfx.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////
#define PROGRAMBLACKLIST                "Cheat Engine 6.2","Cheat Engine 6.1","Extreme Injector", "Inject0r", "shotbot", "Injector by Gordozika","shobot","BOORIS", "Injector de Dll's", "DX3DRipper","Shobo", "Extreme Injector v2.1 by master131", "WutHack", "Hitbox 3.5", "Inject Autorizado Dropeado"//
///////////////////////////////////////////////////////////////////////////////////////////////////////

HMODULE hBaseAddress;
HANDLE hProcess;

bool ProgramDetected = false;
unsigned long old2Protect;

BOOL CALLBACK EnumWindowsHandler(HWND hwnd, bool* retVal){
	char title[0x1000];
	GetWindowText(hwnd, title, sizeof (title) / sizeof (title[0]));
	static const char* titleBlacklist[] = { PROGRAMBLACKLIST };
	for(size_t i = 0; i < sizeof (titleBlacklist) / sizeof (titleBlacklist[0]); ++i){
		if(StrStrI(title, titleBlacklist[i])){
			*retVal = true;
			return FALSE;
		}
		return TRUE;
	}
	return TRUE;
}
bool IsAddressHooked(unsigned long address){
	BYTE* offsetValue = (BYTE*)address;
	return (*offsetValue == 0xE8 || *offsetValue == 0xE9 || *offsetValue == 0x7E || *offsetValue == 0x74 || *offsetValue == 0xFF);
}

void HACKING(){
	DWORD getTickCount = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetTickCount");
	DWORD queryPerformanceCounter = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"), "QueryPerformanceCounter");
	while(1){
	EnumWindows((WNDENUMPROC)EnumWindowsHandler, (LPARAM)(&ProgramDetected));
	if (IsAddressHooked(getTickCount) || IsAddressHooked(queryPerformanceCounter) || GetModuleHandleA ("detoured.dll") != NULL || GetModuleHandleA ("idwmapi.dll") != NULL || GetModuleHandleA ("injector.dll") != NULL || GetModuleHandleA ("proxy.dll") != NULL || GetModuleHandleA ("softsp.dll") != NULL || GetModuleHandleA ("Wall by Zoreh.dll") != NULL || GetModuleHandleA ("Wall Hack ByEvilgamerz.dll")  || GetModuleHandleA ("NOWALLS.dll") != NULL || GetModuleHandleA ("hook.dll") != NULL || GetModuleHandleA ("shellapi.dll") != NULL)
    {
	   ZApplication::GetGameInterface()->ShowWidget("HackWarnings", true, true);
	   ZPostDisconnect();
	  
	  }else if(ProgramDetected == true){
	  ZApplication::GetGameInterface()->ShowWidget("HackWarnings", true, true);
	  ZPostDisconnect();
	  }
	Sleep(10000);
  }
}

    DWORD g_dwLoadLibraryAJMP;

    /* HOOK FUNCTION */

    DWORD WINAPI jumphook( DWORD AddressToPerformJump, DWORD AddressOfMyFunction, DWORD LenghOfTheAreaToPerformTheJump	)
    {
    	if( LenghOfTheAreaToPerformTheJump < 5 )
    		return 0;

    	DWORD RelativeJump, 
    		  NextInstructionAddress,
    		  Flag;

    	if ( ! VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, PAGE_EXECUTE_READWRITE, &Flag) )
    		return 0;

    	NextInstructionAddress = AddressToPerformJump + LenghOfTheAreaToPerformTheJump;

    	*(BYTE*)AddressToPerformJump = 0xE9;

    	for( DWORD i = 5; i < LenghOfTheAreaToPerformTheJump; i++)
    		*(BYTE*)(AddressToPerformJump+i) = 0x90;

    	RelativeJump = AddressOfMyFunction - AddressToPerformJump - 0x5;

    	*(DWORD*)(AddressToPerformJump + 0x1) = RelativeJump;

    	VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, Flag, &Flag);

    	return NextInstructionAddress; 
    }

    /* END HOOK FUNCTION */

    HMODULE WINAPI hLoadLibraryA( LPCSTR lpLibFileName )
    {	
    	__asm
    	{
    		mov eax, dword ptr ss:[esp + 0x18]
    		cmp dword ptr ds:[eax-0x12], 0x8B55FF8B
    		je erro
    	}
    	

    	if( lpLibFileName )
    	{
    		if( !strcmp( lpLibFileName, "twain_32.dll" ) )
    			__asm jmp g_dwLoadLibraryAJMP
    	}			

    	return LoadLibraryExA( lpLibFileName, 0, 0 );

    erro:

    	/* dll injetada */


    	ExitProcess( 0 );

    	return 0;
    }

    void ZPerformHooks()
    {
    	g_dwLoadLibraryAJMP = (DWORD)GetModuleHandle( "kernel32" ) + 0x6E2A1;

    	jumphook( (DWORD)LoadLibraryA, (DWORD)&hLoadLibraryA, 57 );
    }

