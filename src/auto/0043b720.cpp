#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(char, __fastcall, C_0083e9d0_FindMatchingEntry, 0x004E7220, (C_0083e9d0* self, void* edx, int slotIndex, int matchId, int matchRecord));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx, float a3, uint32_t a4, float argC));
CSF_FUNC(int*, __fastcall, MinimapRadar_DrawMarkerAt, 0x0043B530, (void* self, void* edx, float* a2, float a3, void* a4, float argC));

static uint32_t __fastcall C_00830e5c_DrawMarkerForFlaggedObject(C_00830e5c* self, void* /*edx*/, C_008342e8* a2)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t result = static_cast<uint32_t>(GetGlobalValue_8BC1E0());

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x225C))
    {
        if (sceneMenuContext)
        {
            if (a2)
            {
                result = CSF_AT(uint32_t, a2, 0x170);
                if ((result & 0x2000) != 0 && CSF_AT(int, a2, 0x1D8) == 2)
                {
                    char matched = C_0083e9d0_FindMatchingEntry(
                        reinterpret_cast<C_0083e9d0*>(CSF_AT(uint32_t, a2, 0x3C)),
                        nullptr,
                        2,
                        7,
                        static_cast<int>(sceneMenuContext + 28u));
                    result = static_cast<uint8_t>(matched);
                    if (matched)
                    {
                        float a3 = static_cast<float>(C_008342e8_ComputeResolvedObjectScalar(a2, nullptr));
                        float* objectPosition = GetObjectPositionVector(a2, nullptr, a3, 0xFu, 255.0f);
                        int* drawResult = MinimapRadar_DrawMarkerAt(
                            self,
                            nullptr,
                            objectPosition,
                            a3,
                            reinterpret_cast<void*>(static_cast<uintptr_t>(0xF)),
                            255.0f);
                        return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(drawResult));
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0043B720, C_00830e5c_DrawMarkerForFlaggedObject, "auto");
