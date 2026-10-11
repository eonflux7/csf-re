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

// Callee: sub_745350, __thiscall (self in ecx, a2 on stack), ret 4.
CSF_FUNC(char, __fastcall, DdlSpawn_ApplyFallbackFlag, 0x00745350, (void* self, void* edx, char a2));

// Hook target: 0x007410F0, __thiscall with one stack argument (ret 4).
static char __fastcall C_00858180_ApplySpawnFlag(C_00858180* self, void* /*edx*/, int spawnValue)
{
    // Embedded sub-object at +0x6B0: call its vtable slot 2 (offset 8) with this = self + 0x6B0.
    auto subObjectCheck = reinterpret_cast<char(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0x6B0)[2]);
    if (!subObjectCheck(reinterpret_cast<uint8_t*>(self) + 0x6B0, nullptr))
    {
        char fallbackResult = DdlSpawn_ApplyFallbackFlag(self, nullptr, static_cast<char>(spawnValue));
        CSF_AT(char, self, 0x749) = 1;
        return fallbackResult;
    }

    char result = CSF_AT(char, self, 0x749);
    if (!result && CSF_AT(int, self, 0x668) == -1)
    {
        result = static_cast<char>(spawnValue);
        if (spawnValue != CSF_AT(int, self, 0x668))
        {
            CSF_AT(int, self, 0x668) = spawnValue;
        }
        CSF_AT(char, self, 0x749) = 1;
        return result;
    }
    return result;
}

HOOK(0x007410F0, C_00858180_ApplySpawnFlag, "auto");
