#include "injector.h"

void run_time( HMODULE module ) {
	AllocConsole( );
	freopen_s( reinterpret_cast< FILE** >( stdin ), "CONIN$", "r", stdin );
	freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );
	SetConsoleTitleA( "[~] mono injector" );

	printf("[~] press any key.\n");
	std::cin.get();

	injector::inject();

	FreeConsole( );
	FreeLibraryAndExitThread( module, EXIT_SUCCESS );
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,_In_ DWORD fdwReason, _In_ LPVOID lpvReserved ) {
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls( hinstDLL );
		
		run_time( hinstDLL );
	}
	return TRUE;
}