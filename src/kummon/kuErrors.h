#pragma once
//Вывод ошибок

#include "ofMain.h"

//Мы в процессе быстрого завершения программы (например, после kuAssertCritical)
//(OF вызывает exit при этом - и нужно знать это, чтобы не сохранять переменные)
extern bool KuError_emergent_exit;

#define kuAssertWarning( val, message ) \
{ if (!(val)) ofSystemAlertDialog( message ); }

#define kuAssertCritical( val, message ) \
{ if (!(val)) { ofSystemAlertDialog( message ); KuError_emergent_exit = true; OF_EXIT_APP(0);} }

#define kuExit( message ) \
{ ofSystemAlertDialog( message ); KuError_emergent_exit = true; OF_EXIT_APP(0); }