#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

using LPCWSTR = const wchar_t*;

CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(S_004f21d0*, __cdecl, CoreVariantValues_FormatWideString, 0x006966E0, (S_004f21d0* destination, char sourceTag, void** sourceStorage, int sourceWord8, int sourceWord12, int sourceWord16, int sourceLength, unsigned int sourceCapacity, int* arguments));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(void*, g_storedWindowHandle, 0x008BAFD0);

// Import slot of MessageBoxW (called indirectly through the IAT in the original).
using MessageBoxWFunc = int(__stdcall*)(void* hwnd, LPCWSTR text, LPCWSTR caption, unsigned int type);
CSF_GLOBAL(MessageBoxWFunc, g_MessageBoxWImport, 0x0082D290);

static LPCWSTR __cdecl Dialog_ShowLocalizedMessage(LPCWSTR lpCaption, wchar_t* String, char a3)
{
    // Scratch wide-string object (frame ebp-0x40, 0x1C bytes) that ConstructWideString fills in.
    alignas(8) uint8_t stringObjectBytes[0x1C];
    // One contiguous VC7.1 wstring, not independent C++ locals: the formatter
    // writes +4..+0x1A through the destination pointer. Local declaration order
    // does not guarantee the original frame's layout.
    legacy::wstring formatted{};
    formatted.res = 7;
    // Address-of-argument slot (frame ebp-0x4): holds the address of the variadic argument a3.
    int argumentSlot = static_cast<int>(reinterpret_cast<uintptr_t>(&a3));

    ConstructWideString(stringObjectBytes, nullptr, String);

    CoreVariantValues_FormatWideString(
        reinterpret_cast<S_004f21d0*>(&formatted),
        CSF_AT(char, stringObjectBytes, 0x0),
        CSF_AT(void**, stringObjectBytes, 0x4),
        CSF_AT(int, stringObjectBytes, 0x8),
        CSF_AT(int, stringObjectBytes, 0xC),
        CSF_AT(int, stringObjectBytes, 0x10),
        CSF_AT(int, stringObjectBytes, 0x14),
        CSF_AT(unsigned int, stringObjectBytes, 0x18),
        &argumentSlot);

    LPCWSTR messageText = formatted.res < 8 ? formatted.bx.buf : formatted.bx.ptr;
    argumentSlot = 0;

    LPCWSTR result = reinterpret_cast<LPCWSTR>(
        g_MessageBoxWImport(g_storedWindowHandle, messageText, lpCaption, 0x30u));

    if (formatted.res >= 8)
    {
        result = formatted.bx.ptr;
        if (formatted.bx.ptr)
            return reinterpret_cast<LPCWSTR>(static_cast<intptr_t>(Mem_FreeDebug(formatted.bx.ptr)));
    }
    return result;
}

HOOK(0x0069CE50, Dialog_ShowLocalizedMessage, "auto");
