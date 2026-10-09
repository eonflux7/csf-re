#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneHandle_GetPosition, 0x004760B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, SceneHandle_GetDirection, 0x00476100, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Scene_UpdateLinkedPosition(void* self, void* /*edx*/)
{
    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x1B4));

    void* receiver = resolved
        ? CSF_AT(void*, reinterpret_cast<void*>(resolved), 0x34)
        : nullptr;

    if (!receiver || !CSF_AT(std::uint8_t, self, 0x3A5))
        return resolved;

    void* provider = CSF_AT(void*, reinterpret_cast<void*>(resolved), 0x44);
    void* handle =
        ((void* (__fastcall*)(void*, void*, int))
            CSF_AT(void**, provider, 0)[0xC8 / 4])(provider, nullptr, 0);

    void* values = reinterpret_cast<void*>(Handle_GetValuePointer(handle, nullptr));

    float position[3];
    float offset[3];

    if (CSF_AT(std::uint8_t, self, 0x228))
    {
        float origin[3] = {};
        float direction[3] = {};

        SceneHandle_GetPosition(
            handle, nullptr, reinterpret_cast<uint32_t*>(origin));
        SceneHandle_GetDirection(
            handle, nullptr, reinterpret_cast<uint32_t*>(direction));

        const float distance = CSF_AT(float, values, 0xEC);
        offset[1] = direction[1] * distance;
        offset[2] = direction[2] * distance;
        position[0] = direction[0] * distance + origin[0];
        position[1] = offset[1] + origin[1];
        position[2] = offset[2] + origin[2];

        return ((int (__fastcall*)(void*, void*, void*, float*))
            CSF_AT(void**, receiver, 0)[0xC4 / 4])(
                receiver, nullptr, self, position);
    }

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* cachedProvider =
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    void* referencedObject = GetReferencedObject(cachedProvider, nullptr);

    void* transformProvider =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                referencedObject, nullptr);

    std::uint32_t matrixState[3] = {};
    void* matrixResult =
        ((void* (__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, transformProvider, 0)[0x8C / 4])(
                transformProvider, nullptr, matrixState);

    const float distance = CSF_AT(float, values, 0xEC);
    float* matrix = CSF_AT(float*, matrixResult, 0);
    offset[0] = distance * CSF_AT(float, matrix, 0x20);
    offset[1] = distance * CSF_AT(float, matrix, 0x24);
    offset[2] = distance * CSF_AT(float, matrix, 0x28);

    float positionScratch[3];
    float* origin =
        ((float* (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, self, 0)[0xA4 / 4])(
                self, nullptr, positionScratch);

    position[2] = offset[2] + origin[2];
    position[1] = offset[1] + origin[1];
    position[0] = offset[0] + origin[0];

    if (CSF_AT(std::uint8_t, matrixState, 0x4))
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixState[0]));

    return ((int (__fastcall*)(void*, void*, void*, float*))
        CSF_AT(void**, receiver, 0)[0xC4 / 4])(
            receiver, nullptr, self, position);
}

HOOK(0x00496B30, Scene_UpdateLinkedPosition, "auto");
