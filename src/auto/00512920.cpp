#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// AssignStringSubstring: thiscall, ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
// sub_43EB30: thiscall, ret 8
CSF_FUNC(void*, __fastcall, sub_43EB30, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
// Mem_FreeDebug: cdecl, ret 0
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Copies a2 into a temporary string, runs it through sub_43EB30 together with
// the a3 text, then assigns the transformed result into a1.
static int __cdecl ProfileSaveGame_AssignTransformedName(C_0082d410* a1, C_0082d410* a2, uint32_t* a3)
{
    // Temporary string object kept on the stack (0x1C bytes in the original frame).
    alignas(8) uint8_t tempBytes[0x1C];
    CSF_AT(uint32_t, tempBytes, 0x18) = 15;   // capacity
    CSF_AT(uint32_t, tempBytes, 0x14) = 0;    // length
    CSF_AT(uint8_t, tempBytes, 4) = 0;        // first byte cleared

    AssignStringSubstring(tempBytes, nullptr, a2, 0, 0xFFFFFFFFu);

    void* transformed = sub_43EB30(tempBytes, 0, a3, static_cast<unsigned int>(std::strlen(reinterpret_cast<const char*>(a3))));

    // a1[2].__vftable = 15  -> a1 + 0x18
    CSF_AT(uint32_t, a1, 0x18) = 15;
    // a1[1].field_0008 = nullptr -> a1 + 0x14
    CSF_AT(uint32_t, a1, 0x14) = 0;
    CSF_AT(uint8_t, a1, 4) = 0;

    AssignStringSubstring(a1, nullptr, transformed, 0, 0xFFFFFFFFu);

    if (CSF_AT(uint32_t, tempBytes, 0x18) >= 0x10 && CSF_AT(void*, tempBytes, 4) != nullptr)
    {
        Mem_FreeDebug(CSF_AT(void*, tempBytes, 4));
    }

    return reinterpret_cast<int>(a1);
}

HOOK(0x512920, ProfileSaveGame_AssignTransformedName, "auto");
