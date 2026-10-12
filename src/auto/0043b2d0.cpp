#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, MinimapRadar_ComputeBlipPosition, 0x0043A9C0, (void* self, void* edx, float* a2, float* a3, float* a4, float a5, float a6));
CSF_FUNC(void, __cdecl, MinimapRadar_PrepareEntry, 0x0043AA80, (int* a1));
CSF_FUNC(char, __fastcall, MinimapRadar_AddBlip, 0x0043AF30, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall Minimap_UpdateObjectBlip(C_00830e5c* self, void* /*edx*/, C_008342e8* obj, char flag, float param)
{
    C_008342e8* sceneMenuContext = reinterpret_cast<C_008342e8*>(GetSceneMenuContext(g_pCurrentScene, nullptr));

    int* entry = ((int*(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr, obj);

    char result = 0;
    if (obj) {
        if (!entry)
            return 0;
        MinimapRadar_PrepareEntry(entry);
        self->vSlot8(obj, (int)(intptr_t)entry);
        result = (char)self->vSlot10(reinterpret_cast<uint32_t*>(obj), entry);
    }

    if (entry) {
        float value = (float)self->vGetValue();
        float* objPos = GetObjectPositionVector(obj, nullptr);
        float* ctxPos = GetObjectPositionVector(sceneMenuContext, nullptr);

        float resultPos;
        MinimapRadar_ComputeBlipPosition(self, nullptr, &resultPos, ctxPos, objPos, value, param);

        alignas(4) uint8_t blipBytes[0x14];
        CSF_AT(int, blipBytes, 0x00) = (int)(intptr_t)entry;
        CSF_AT(float, blipBytes, 0x04) = (float)C_008342e8_ComputeResolvedObjectScalar(obj, nullptr);
        CSF_AT(float, blipBytes, 0x08) = resultPos;
        CSF_AT(C_008342e8*, blipBytes, 0x0C) = sceneMenuContext;
        CSF_AT(char, blipBytes, 0x10) = flag;
        CSF_AT(char, blipBytes, 0x11) = 1;

        result = MinimapRadar_AddBlip(self, nullptr, reinterpret_cast<float*>(blipBytes));
    }
    return result;
}

HOOK(0x0043B2D0, Minimap_UpdateObjectBlip, "auto");
