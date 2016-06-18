#pragma once
//Показать/скрыть консоль (Windows)

//For Windows, in ofConstants.h:
//	#ifndef _WIN32_WINNT
//		//#   define _WIN32_WINNT 0x400
//	#define _WIN32_WINNT 0x500

#include "ofMain.h"

bool console_working();				//работает ли тут консоль
void console_show();				//показать консоль
void console_hide();				//скрыть консоль

