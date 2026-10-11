#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char*, __fastcall, GetNeighborList, 0x00445A60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __fastcall, ProcessNeighborObject, 0x004E5E10, (void* self, void* edx, int* a2, void* a3));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void*, float* out));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

static int __fastcall S_004e5640_ScanNeighborObjects(S_004e5640* self, void* /*edx*/)
{
    alignas(8) uint8_t dirBytes[12];
    alignas(8) uint8_t tmpBytes[8];
    alignas(8) uint8_t vecBytes[8];
    int* dir = reinterpret_cast<int*>(dirBytes);
    int* vec = reinterpret_cast<int*>(vecBytes);

    void** vtbl = reinterpret_cast<void**>(self->field_0000);
    ((void(__fastcall*)(void*, void*))vtbl[0xB0 / 4])(self, nullptr);

    vtbl = reinterpret_cast<void**>(self->field_0000);
    float* r = ((float*(__fastcall*)(void*, void*, void*))vtbl[0x8C / 4])(self, nullptr, tmpBytes);
    GfxMatrix_EulerAnglesToDirection(r, nullptr, reinterpret_cast<float*>(dirBytes));

    vec[0] = dir[0];
    vec[1] = dir[2];
    NormalizeVector2(vec, nullptr);

    void* scene = g_pCurrentScene;
    uint32_t objA = self->field_0008;
    char* list = GetNeighborList(CSF_AT(void*, scene, 0xB0), nullptr, CSF_AT(int, reinterpret_cast<void*>(objA), 0x78));

    uint32_t count = CSF_AT(uint32_t, list, 8);
    int result = static_cast<int>(count);
    if (count != 0) {
        int offset = 0;
        uint32_t remaining = count;
        do {
            int obj = CHandleTable_Resolve(g_pCurrentScene, nullptr, CSF_AT(int, list, 0) + offset);
            if (obj != 0 && obj != static_cast<int>(self->field_0008) &&
                (CSF_AT(uint8_t, reinterpret_cast<void*>(obj), 0x170) & 0x10) != 0) {
                ProcessNeighborObject(self, nullptr, vec, reinterpret_cast<void*>(obj));
            }
            offset += 12;
            --remaining;
            result = static_cast<int>(remaining);
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x004E5F50, S_004e5640_ScanNeighborObjects, "auto");
