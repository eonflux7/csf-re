#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float payload));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_008342e8_SelectPlacement, 0x0049F220, (void* self, void* edx, int a2));

static C_008342e8* __fastcall AiMountTasks_ApplyPlacementAndDispatchRequest(
    void* self, void* /*edx*/)
{
    auto* placement = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(
                static_cast<uint8_t*>(CSF_AT(void*, self, 0xC)) + 0x100)));

    if (!placement)
        return nullptr;

    C_008342e8_SelectPlacement(
        placement, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0xC), 0x110));

    auto* flags = reinterpret_cast<uint8_t*>(
        C_008342e8_GetControlFlagsAddress(
            placement, nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0xC), 0x110)));

    if ((*flags & 1) != 0)
    {
        void* controller = CSF_AT(
            void*,
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118),
            0x44);

        if (((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[0xD8 / 4])(
                controller, nullptr))
        {
            void* controlledObject =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, controller, 0)[0xD8 / 4])(
                    controller, nullptr);

            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, controlledObject, 0)[0x38 / 4])(
                controlledObject, nullptr, 0);
        }
    }

    alignas(8) uint8_t positionBytes[0x10];
    auto* position = reinterpret_cast<uint32_t*>(positionBytes);
    C_008342e8_CopyControlPosition(
        placement, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0xC), 0x110),
        position);

    void* entries = reinterpret_cast<void*>(placement->vGetField016c());
    int entryIndex = ((int(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, entries, 0)[0x98 / 4])(
        entries, nullptr, position[1]);

    if (entryIndex == -1)
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(
                CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118)),
            nullptr);
    }
    else
    {
        entries = reinterpret_cast<void*>(placement->vGetField016c());
        void* entry = ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x8C / 4])(
            entries, nullptr, entryIndex);

        void* entryData = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);

        int* focus = ((int*(__fastcall*)(void*, void*))
            CSF_AT(void**, entryData, 0)[0x10 / 4])(entryData, nullptr);

        SceneLoader_ConfigureFocusEntry(
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118),
            nullptr, focus);

        entryData = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);

        CSF_AT(float, positionBytes, 0xC) = static_cast<float>(
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, entryData, 0)[0x28 / 4])(
                entryData, nullptr));

        double resolvedScalar =
            C_008342e8_ComputeResolvedObjectScalar(placement, nullptr);
        float payload = static_cast<float>(
            resolvedScalar + CSF_AT(float, positionBytes, 0xC));

        DispatchSceneObjectWithPayload(
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x118),
            nullptr, payload);
    }

    void* recipient = CSF_AT(
        void*, CSF_AT(void*, self, 0x8), 0x118);
    return ((C_008342e8*(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, recipient, 0)[0x8C / 4])(
        recipient, nullptr, 39, 0, -1, 0);
}

HOOK(0x004A7A50, AiMountTasks_ApplyPlacementAndDispatchRequest, "auto");
