#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Text buffer that receives the formatted error message (data at 0x8BAFC0).
CSF_GLOBAL(char, g_ErrorReportBuffer, 0x008BAFC0);

// Window handle used as the owner of the message box.
CSF_GLOBAL(void*, g_storedWindowHandle, 0x008BAFD0);

// _vsprintf (CRT), __cdecl, pops nothing.
CSF_FUNC(int, __cdecl, _vsprintf, 0x007AFCAA, (char* buffer, const char* format, __builtin_va_list args));

// user32!MessageBoxA, reached through the import address table slot at 0x82D2C4. __stdcall, 4 args.
typedef int(__stdcall* MessageBoxAFn)(void* hWnd, const char* lpText, const char* lpCaption, uint32_t uType);

static const char* OverflowWarningText() { return CSF_STR(0x0085E958, "\n------OJO!!!!. EL MENSAJE ES DEMASIADO GRANDE...PROBABLEMENTE SE HA PRODUCIDO CORRUPCION DE MEMORIA\n"); }

static int __cdecl AppMain_ReportInitializationError(const char* lpCaption, char* Format, ...)
{
    __builtin_va_list va;
    __builtin_va_start(va, Format);

    char* textBuffer = &g_ErrorReportBuffer;
    if (_vsprintf(textBuffer, Format, va) > 1)
    {
        strcat(textBuffer, OverflowWarningText());
    }

    __builtin_va_end(va);

    MessageBoxAFn messageBoxA = CSF_AT(MessageBoxAFn, CSF_ADDR_DATA(0x0082D2C4), 0);
    return messageBoxA(g_storedWindowHandle, textBuffer, lpCaption, 0x30u);
}

HOOK(0x0069CDF0, AppMain_ReportInitializationError, "auto");
