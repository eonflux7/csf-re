#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, sub_755880, 0x00755880, (int a1));

// Stores 0 into the float slot of the holder at a1+4, then looks up the handle record
// referenced through a3. If the record is valid (kind >= 0), resolves it through the
// current scene's handle table and, when the scene object passes the 0x755880 check,
// writes its 0x1a0 value (as an unsigned float) into the holder's float slot.
static unsigned int __stdcall ResolveSceneObjectValueToFloat(int a1, int /*a2*/, int a3)
{
    // *(holder + 4) = 0
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a1), 4)), 4) = 0;

    // record = **(a3) + 4 field
    uint8_t* referencedObject = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a3), 0);
    uint32_t recordAddress = CSF_AT(uint32_t, referencedObject, 4);
    int32_t recordKind = CSF_AT(int32_t, reinterpret_cast<uint8_t*>(recordAddress), 12);

    uint32_t result = recordAddress + 4;
    if (recordKind >= 0)
    {
        result = static_cast<uint32_t>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(recordAddress + 4)));
        uint32_t sceneObject = result;
        if (sceneObject != 0)
        {
            result = static_cast<uint32_t>(
                sub_755880(CSF_AT(int32_t, reinterpret_cast<uint8_t*>(sceneObject), 0x50)));
            if (static_cast<uint8_t>(result) != 0)
            {
                uint32_t rawValue = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(sceneObject), 0x1A0);
                int32_t signedValue = static_cast<int32_t>(rawValue);
                float floatValue = static_cast<float>(signedValue);
                if (signedValue < 0)
                    floatValue += g_UnsignedIntegerBias;

                // Reload the holder pointer after the calls above.
                CSF_AT(float, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a1), 4)), 4) = floatValue;
                result = rawValue;
            }
        }
    }
    return result;
}

HOOK(0x0045C2E0, ResolveSceneObjectValueToFloat, "auto");
