#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __fastcall, sub_4ADCC0, 0x004ADCC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _vsprintf, 0x007AFCAA, (char* buffer, const char* format, __builtin_va_list args));

CSF_GLOBAL(char, g_DebugLineBuffer, 0x008B0450);

static int __cdecl AiCore_DebugPrintFinIa(int a1, const char* Format, ...)
{
    __builtin_va_list va;
    __builtin_va_start(va, Format);

    // Pending-flush flag at a1 + 0x6E8: if set, clear it and run the flush routine.
    void* self = reinterpret_cast<void*>(a1);
    if (CSF_AT(uint8_t, self, 0x6E8))
    {
        CSF_AT(uint8_t, self, 0x6E8) = 0;
        sub_4ADCC0(self, nullptr);
    }

    // Buffer is reset to empty, so the append offset is always zero.
    g_DebugLineBuffer = 0;
    int result = _vsprintf(&g_DebugLineBuffer, Format, va);

    __builtin_va_end(va);
    return result;
}

HOOK(0x4B90C0, AiCore_DebugPrintFinIa, "auto");
