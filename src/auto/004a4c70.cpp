#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_GetSceneEntry, 0x0040F790, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, Gui_FindAvailableAction, 0x004A4A40, (void* self, void* edx, void* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Required by the packet's reference list; this address is also the getter above.
[[maybe_unused]] static const auto kListedGetterVtable = CSF_VTBL(0x00492B20);

static S_00492b20** Gui_ActionResult(uint32_t index)
{
    return reinterpret_cast<S_00492b20**>(static_cast<uintptr_t>(index));
}

static S_00492b20** __fastcall Gui_ResolveGameModeAction(
    void* self, void* /*edx*/, CMenuMpGameMode* action)
{
    void* owner = CSF_AT(void*, self, 0x8);
    void* provider = CSF_AT(void*, owner, 0x44);
    void* handle =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0xD8 / 4])(provider, nullptr);

    bool alternate = !handle;
    if (handle)
    {
        void* value = reinterpret_cast<void*>(
            Handle_GetValuePointer(handle, nullptr));
        alternate = CSF_AT(uint32_t, value, 0x74) == 32;
    }

    void* scene = g_pCurrentScene;
    char* controlHandle = S_00492b20_GetPtr0x1B4(owner, nullptr);
    auto* control = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(scene, nullptr,
                             static_cast<int>(reinterpret_cast<uintptr_t>(controlHandle))));
    if (!control)
        return Gui_ActionResult(static_cast<uint32_t>(-1));

    switch (reinterpret_cast<uintptr_t>(action))
    {
    case 0:
    case 10:
    {
        int index = C_Unknown_GetField_01c0(owner, nullptr);
        void* entry = reinterpret_cast<void*>(
            GetControlEntryByIndex(control, nullptr, index));
        if (CSF_AT(uint32_t, entry, 0x4) != CSF_AT(uint32_t, owner, 0x20))
            return Gui_ActionResult(static_cast<uint32_t>(-1));

        void* flags = reinterpret_cast<void*>(
            C_008342e8_GetControlFlagsAddress(control, nullptr, index));
        if (CSF_AT(uint8_t, flags, 0) & 2)
            return Gui_ActionResult(static_cast<uint32_t>(-1));

        uint32_t candidate = static_cast<uint32_t>(index) +
            (action ? 0x14 : 0x10);
        if (static_cast<uint32_t>(index) < 2 &&
            Gui_FindAvailableAction(
                self, nullptr,
                reinterpret_cast<void*>(static_cast<uintptr_t>(candidate))) != -1)
            return Gui_ActionResult(candidate);

        if (!action)
            return Gui_ActionResult(alternate ? 10 : 4);
        return Gui_ActionResult(alternate ? 11 : 5);
    }

    case 8:
        return Gui_ActionResult(
            static_cast<uint32_t>(C_Unknown_GetField_01c0(owner, nullptr)) + 18);

    case 36:
    case 37:
    {
        uint32_t index =
            static_cast<uint32_t>(C_Unknown_GetField_01c0(owner, nullptr));
        void* sceneEntries = reinterpret_cast<void*>(control->field_004c);

        alignas(8) uint8_t positionBytes[0x14];
        auto* position = reinterpret_cast<uint32_t*>(positionBytes);
        int currentIndex = C_Unknown_GetField_01c0(owner, nullptr);
        C_008342e8_CopyControlPosition(control, nullptr, currentIndex, position);
        void* sceneEntry = reinterpret_cast<void*>(
            AiCore_GetSceneEntry(sceneEntries, nullptr,
                                 static_cast<int>(position[0])));

        if (CSF_AT(uint32_t, sceneEntry, 0))
        {
            if (reinterpret_cast<uintptr_t>(action) == 36)
                return Gui_ActionResult(alternate ? 6 : 0);
            return Gui_ActionResult(alternate ? 7 : 1);
        }

        return Gui_ActionResult(
            index + (reinterpret_cast<uintptr_t>(action) == 36 ? 12 : 14));
    }

    case 38:
    {
        uint32_t index =
            static_cast<uint32_t>(C_Unknown_GetField_01c0(owner, nullptr));
        uint32_t candidate = index + 12;
        if (index >= 2 ||
            Gui_FindAvailableAction(
                self, nullptr,
                reinterpret_cast<void*>(static_cast<uintptr_t>(candidate))) == -1)
            return Gui_ActionResult(alternate ? 8 : 2);
        return Gui_ActionResult(candidate);
    }

    case 39:
    {
        uint32_t index =
            static_cast<uint32_t>(C_Unknown_GetField_01c0(owner, nullptr));
        uint32_t candidate = index + 14;
        if (index < 2 &&
            Gui_FindAvailableAction(
                self, nullptr,
                reinterpret_cast<void*>(static_cast<uintptr_t>(candidate))) != -1)
            return Gui_ActionResult(candidate);
        return Gui_ActionResult(alternate ? 9 : 3);
    }

    default:
        return Gui_ActionResult(static_cast<uint32_t>(-1));
    }
}

HOOK(0x004A4C70, Gui_ResolveGameModeAction, "auto");
