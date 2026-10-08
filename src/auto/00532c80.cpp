#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// CRT wcslen, called by the original at 0x7B0499.
CSF_FUNC(size_t, __cdecl, Wcslen_7B0499, 0x007B0499, (const wchar_t* str));

// Internal helper (__thiscall, ret 12) that actually appends/inserts the string.
CSF_FUNC(int, __fastcall, MenuEula_532BC0, 0x00532BC0,
         (void* self, void* /*edx*/, const wchar_t* str, uint64_t value));

// Computes the length of the string and forwards it (packed as the high dword of a
// 64-bit value, with a3 as the low dword) to the internal insert helper.
static int __fastcall MenuEula_AddStringWithLength(void* self, void* /*edx*/,
                                                   wchar_t* String, unsigned int a3)
{
    const size_t length = Wcslen_7B0499(String);
    const uint64_t packed = (static_cast<uint64_t>(length) << 32) | a3;
    return MenuEula_532BC0(self, nullptr, String, packed);
}

HOOK(0x00532C80, MenuEula_AddStringWithLength, "auto");
