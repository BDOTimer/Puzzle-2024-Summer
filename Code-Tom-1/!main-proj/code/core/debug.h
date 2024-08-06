#ifndef DEBUG_H
#define DEBUG_H
///----------------------------------------------------------------------------|
/// "debug.h"
/// отключить консольное окно: -mwindows
///----------------------------------------------------------------------------:
#include <iostream>

#define l(v)   std::wcout << #v << " = " << (v) << "\n";
#define SIGNAL std::wcout << L"SIGNAL ..." << "\n";

#endif // DEBUG_H
