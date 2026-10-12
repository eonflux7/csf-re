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

CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(bool, __cdecl, LightObjectCheckAgainstEntry, 0x006D0F40, (void* a1, float* a2));

// Detaches a light object from its RpWorld (if it has one), then runs its
// per-entry check over the stored list. Returns the low byte of the working
// accumulator, as the original does via LOBYTE/al.
static char __fastcall C_00860e68_DetachLightFromWorld(C_00860e68* self, void* /*edx*/, C_0085e7b8* a2)
{
    int eax = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, a2, 0)[6])(a2, nullptr);
    if (eax)
    {
        eax = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, a2, 0)[6])(a2, nullptr);
        if (eax != 1)
        {
            void* lightHandle = GetCachedObjectProvider(a2, nullptr);
            eax = static_cast<int>(reinterpret_cast<intptr_t>(lightHandle));
            if (lightHandle)
            {
                eax = RpLightGetWorld(static_cast<int>(reinterpret_cast<intptr_t>(lightHandle)));
                if (eax)
                {
                    int removed = RpWorldRemoveLight(eax, static_cast<int>(reinterpret_cast<intptr_t>(lightHandle)));
                    eax = (eax & ~0xFF) | (removed & 0xFF);
                }
            }

            uint32_t index = 0;
            do
            {
                if (index >= self->field_0090)
                    break;
                float* entry = reinterpret_cast<float**>(self->field_0088)[index];
                index++;
                bool passed = LightObjectCheckAgainstEntry(a2, entry);
                eax = (eax & ~0xFF) | (!passed ? 1 : 0);
            } while ((eax & 0xFF) != 0);
        }
    }
    return static_cast<char>(eax);
}

HOOK(0x006D1D30, C_00860e68_DetachLightFromWorld, "auto");
