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
    // Destination for the formatted text (frame ebp-0x20).
    int formatDestination = 0;
    // Inline text buffer / pointer to the formatted text (frame ebp-0x1C..ebp-0xC).
    LPCWSTR lpText[5];
    // Buffer capacity marker (frame ebp-0x8); >= 8 means lpText[0] is a heap pointer.
    uint32_t bufferCapacity = 7;
    // Address-of-argument slot (frame ebp-0x4): holds the address of the variadic argument a3.
    int argumentSlot = static_cast<int>(reinterpret_cast<uintptr_t>(&a3));

    lpText[4] = nullptr;
    lpText[0] = nullptr;

    ConstructWideString(stringObjectBytes, nullptr, String);

    CoreVariantValues_FormatWideString(
        reinterpret_cast<S_004f21d0*>(&formatDestination),
        CSF_AT(char, stringObjectBytes, 0x0),
        CSF_AT(void**, stringObjectBytes, 0x4),
        CSF_AT(int, stringObjectBytes, 0x8),
        CSF_AT(int, stringObjectBytes, 0xC),
        CSF_AT(int, stringObjectBytes, 0x10),
        CSF_AT(int, stringObjectBytes, 0x14),
        CSF_AT(unsigned int, stringObjectBytes, 0x18),
        &argumentSlot);

    LPCWSTR messageText = lpText[0];
    argumentSlot = 0;
    if (bufferCapacity < 8)
        messageText = reinterpret_cast<LPCWSTR>(lpText);

    LPCWSTR result = reinterpret_cast<LPCWSTR>(
        g_MessageBoxWImport(g_storedWindowHandle, messageText, lpCaption, 0x30u));

    if (bufferCapacity >= 8)
    {
        result = lpText[0];
        if (lpText[0])
            return reinterpret_cast<LPCWSTR>(static_cast<intptr_t>(Mem_FreeDebug(reinterpret_cast<void*>(const_cast<wchar_t*>(lpText[0])))));
    }
    return result;
}

HOOK(0x0069CE50, Dialog_ShowLocalizedMessage, "auto");
