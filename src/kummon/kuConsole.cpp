#include "kuConsole.h"

#ifdef TARGET_WIN32
#include <windows.h>
#endif


//-------------------------------------------------------------------
bool console_working() {			//работает ли тут консоль
#ifdef TARGET_WIN32
	return true;
#else
	return false;
#endif

}

//-------------------------------------------------------------------
void console_show()					//показать консоль
{
#ifdef TARGET_WIN32
	HWND hWnd = GetConsoleWindow();
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
#endif
}

//-------------------------------------------------------------------
void console_hide()					//скрыть консоль
{
#ifdef TARGET_WIN32
	HWND hWnd = GetConsoleWindow();
	ShowWindow( hWnd, SW_HIDE );
#endif
}


//-------------------------------------------------------------------
void window_activate() {
#ifdef TARGET_WIN32
	SetForegroundWindow(GetActiveWindow());
#endif
}

//-------------------------------------------------------------------
string systemTextBoxDialog(const string &title, const string &value)
{
	string result = ofSystemTextBoxDialog( title, value );
	window_activate();
	return result;
}

//-------------------------------------------------------------------
