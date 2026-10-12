#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int*, __fastcall, Minimap_DrawBlip, 0x0043B530, (void* self, void* edx, float* a2, float a3, void* a4, float argC));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx, float a));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void* edx));

static int* __fastcall Minimap_DrawRadarObjects(C_00830e5c* self, void* /*edx*/)
{
    int* result = reinterpret_cast<int*>(self->field_0004);
    if (!result)
        return result;

    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);

    uint8_t* edi = reinterpret_cast<uint8_t*>(self->field_0004) + 8;
    int count = 0;
    if (CSF_AT(int, edi, 4) != 0)
        count = (CSF_AT(int, edi, 8) - CSF_AT(int, edi, 4)) / 40;

    if (static_cast<uint32_t>(count) > 0)
    {
        int offset = 0;
        int remaining = count;
        do
        {
            int* obj = reinterpret_cast<int*>(CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                                   CSF_AT(int, edi, 4) + offset + 0x1C));
            if (obj)
            {
                void** vtbl = CSF_AT(void**, obj, 0);
                int flag = ((int(__fastcall*)(void*, void*))vtbl[0x28 / 4])(obj, nullptr);
                if (flag && reinterpret_cast<uint32_t>(obj) != sceneContext)
                {
                    uint8_t* entries = CSF_AT(uint8_t*, edi, 4);
                    float extra = CSF_AT(float, entries, offset + 4);
                    void* ptr = CSF_AT(void*, entries, offset);
                    float scalar = static_cast<float>(
                        C_008342e8_ComputeResolvedObjectScalar(reinterpret_cast<C_008342e8*>(obj), nullptr));
                    float* pos = GetObjectPositionVector(obj, nullptr, scalar);
                    Minimap_DrawBlip(self, nullptr, pos, scalar, ptr, extra);
                }
            }
            offset += 40;
            --remaining;
        } while (remaining != 0);
    }

    uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0004);
    uint8_t* list = base + 0x18;
    int count2 = CSF_AT(int, base, 0x1C);
    result = reinterpret_cast<int*>(static_cast<intptr_t>(count2));
    if (count2 != 0)
        count2 = (CSF_AT(int, list, 8) - count2) / 40;
    result = reinterpret_cast<int*>(static_cast<intptr_t>(count2));

    if (static_cast<uint32_t>(count2) > 0)
    {
        int offset = 0;
        int remaining = count2;
        do
        {
            uint8_t* entries = CSF_AT(uint8_t*, list, 4);
            float extra = CSF_AT(float, entries, offset + 4);
            void* ptr = CSF_AT(void*, entries, offset);
            float* pos = reinterpret_cast<float*>(entries + offset + 0x1C);
            result = Minimap_DrawBlip(self, nullptr, pos, -1.0f, ptr, extra);
            offset += 40;
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x0043B7A0, Minimap_DrawRadarObjects, "auto");
