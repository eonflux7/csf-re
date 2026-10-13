#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AppendStringSubstring, 0x0043EA30, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Builds tmp = a2 + a3 (string append), then assigns the resulting text into a1 and returns a1.
static int __cdecl ErrorLog_ConcatStrings(C_0082d410* a1, C_0082d410* a2, uint32_t* a3)
{
    // Local std::string-like temporary (0x1C bytes): +4 data pointer, +0x14 size, +0x18 capacity.
    alignas(8) uint8_t tmpStringBytes[0x1C];

    CSF_AT(uint32_t, tmpStringBytes, 0x18) = 15;
    CSF_AT(uint32_t, tmpStringBytes, 0x14) = 0;
    CSF_AT(uint8_t, tmpStringBytes, 4) = 0;
    AssignStringSubstring(tmpStringBytes, nullptr, a2, 0, 0xFFFFFFFFu);

    void* appended = AppendStringSubstring(tmpStringBytes, nullptr, a3, 0, 0xFFFFFFFFu);

    CSF_AT(uint32_t, a1, 0x18) = 15;
    CSF_AT(uint32_t, a1, 0x14) = 0;
    CSF_AT(uint8_t, a1, 4) = 0;
    AssignStringSubstring(a1, nullptr, appended, 0, 0xFFFFFFFFu);

    if (CSF_AT(uint32_t, tmpStringBytes, 0x18) >= 0x10 && CSF_AT(void*, tmpStringBytes, 4) != nullptr)
    {
        Mem_FreeDebug(CSF_AT(void*, tmpStringBytes, 4));
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(a1));
}

HOOK(0x0069D000, ErrorLog_ConcatStrings, "auto");
