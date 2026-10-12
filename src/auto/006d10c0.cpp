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

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00861988_SetSecondaryParameters, 0x006FD040, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861988_SetTertiaryParameters, 0x006FD080, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, GfxLights_ClearLightFlag, 0x006FD0C0, (void* self, void* edx, char a2));

CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);

// Applies the tertiary/secondary light parameters to the world light object,
// clears the stale object-provider slots in the first array of a2, and zeroes
// the second array (a2[10..]). Returns the number of zeroed entries.
static unsigned int __fastcall GfxLights_ApplyLightParamsAndClearSlots(
    S_006d10c0* self, void* /*edx*/, uint32_t* a2, char a3, int a4, int a5, int a6)
{
    (void)a4;
    (void)a5;
    (void)a6;

    void* lightObject = reinterpret_cast<void*>(self->field_0030);

    C_00861988_SetTertiaryParameters(lightObject, nullptr, reinterpret_cast<uint32_t*>(&a3));

    lightObject = reinterpret_cast<void*>(self->field_0030);
    C_00861988_SetSecondaryParameters(lightObject, nullptr, reinterpret_cast<uint32_t*>(&g_ColorWhite));

    lightObject = reinterpret_cast<void*>(self->field_0030);
    GfxLights_ClearLightFlag(lightObject, nullptr, 0);

    uint32_t providerIndex = 0;
    if (a2[0] != 0) {
        uint32_t* providerSlot = a2 + 2;
        do {
            void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(self->field_0030), nullptr);
            if (*providerSlot != reinterpret_cast<uint32_t>(provider)) {
                *providerSlot = 0;
            }
            ++providerSlot;
            ++providerIndex;
        } while (providerIndex < a2[0]);
    }

    unsigned int result = 0;
    if (a2[1] != 0) {
        uint32_t* secondSlot = a2 + 10;
        do {
            *secondSlot = 0;
            ++result;
            ++secondSlot;
        } while (result < a2[1]);
    }

    return result;
}

HOOK(0x006D10C0, GfxLights_ApplyLightParamsAndClearSlots, "auto");
