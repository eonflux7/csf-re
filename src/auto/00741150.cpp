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

CSF_FUNC(char, __fastcall, sub_744E00, 0x00744E00, (void* self, void* edx, char a2));

// Called from the original as __thiscall with one stack argument (ret 4).
static char __fastcall SpawnPoints_UpdateCharacter(C_00858180* self, void* /*edx*/, int a2)
{
    // Embedded sub-object at self+0x6B0; its vtable slot 2 is queried.
    void* embedded = reinterpret_cast<uint8_t*>(self) + 0x6B0;
    char result = ((char (__fastcall*)(void*, void*))CSF_AT(void**, self, 0x6B0)[2])(embedded, nullptr);

    if (result == 0)
        return sub_744E00(self, nullptr, static_cast<char>(a2));

    int currentSlot = CSF_AT(int, self, 0x668);
    if (currentSlot == a2)
    {
        result = -1;
        if (currentSlot != -1)
            CSF_AT(int, self, 0x668) = -1;
    }
    return result;
}

HOOK(0x00741150, SpawnPoints_UpdateCharacter, "auto");
