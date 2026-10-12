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

CSF_FUNC(float*, __fastcall, Minimap_ComputeBlipPosition, 0x0043A9C0, (void* self, void* edx, float* a2, float* a3, float* a4, float a5, float a6));
CSF_FUNC(int, __fastcall, Minimap_IsPointVisible, 0x0043AAD0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, Minimap_DrawBlip, 0x0043AF30, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Minimap_DrawAllBlips(C_00830e5c* self, void* /*edx*/)
{
    int base = GetValue8B0554() + 84;
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    int globalValue = GetGlobalValue_44ACE0();

    int begin = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), 24);
    uint8_t* list = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalValue)) + 20;

    int count = 0;
    if (begin) {
        count = (CSF_AT(int, list, 8) - begin) / 12;
    }
    if (count == 0) {
        return 0;
    }

    int offset = 0;
    int remaining = count;
    do {
        float* entry = reinterpret_cast<float*>(CSF_AT(uint8_t*, list, 4) + offset);

        int visible = Minimap_IsPointVisible(self, nullptr, entry);
        int flagInt = ((uint8_t)visible == 0) ? 1 : 0;
        float flag;
        std::memcpy(&flag, &flagInt, sizeof(flag));

        float scale = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(base)), 0x920);
        float* posVec = GetObjectPositionVector(reinterpret_cast<void*>(static_cast<uintptr_t>(sceneMenuContext)), nullptr);

        alignas(8) uint8_t outBytes[0x18];
        float* out = reinterpret_cast<float*>(outBytes);
        Minimap_ComputeBlipPosition(self, nullptr, out, posVec, entry, scale, flag);

        uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
        uint32_t value = CSF_AT(uint32_t, owner, 0x40);
        if (value) {
            alignas(8) uint8_t blipBytes[0x14];
            CSF_AT(uint32_t, blipBytes, 0x0) = value;
            CSF_AT(uint32_t, blipBytes, 0x4) = 0;
            CSF_AT(float, blipBytes, 0x8) = out[0];
            CSF_AT(float, blipBytes, 0xC) = out[1];
            CSF_AT(uint8_t, blipBytes, 0x10) = 0;
            CSF_AT(uint8_t, blipBytes, 0x11) = 1;
            Minimap_DrawBlip(self, nullptr, reinterpret_cast<float*>(blipBytes));
        }

        offset += 12;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x0043D3C0, Minimap_DrawAllBlips, "auto");
