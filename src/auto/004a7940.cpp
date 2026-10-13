#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (C_008342e8* self, void* , int a2));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_CopyFocusEntryPosition, 0x0049EE00, (C_008342e8* self, void* , int entryIndex, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static hook::Tail __fastcall AiMountTasks_ApplyFocusEntryAndUpdate(
    void* self, const uint32_t* /*args*/)
{
    auto* focusOwner = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x0c) + 256)));

    if (!focusOwner)
        return hook::tail_ret(0);

    const int flagsAddress = C_008342e8_GetControlFlagsAddress(
        focusOwner, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x110));

    if ((CSF_AT(uint8_t, reinterpret_cast<void*>(flagsAddress), 0) & 1) != 0)
    {
        void* controller = CSF_AT(
            void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), 0x44);

        void** controllerVtable;
        std::memcpy(&controllerVtable, controller, sizeof(controllerVtable));
        const int activeObjectAddress =
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                controllerVtable[0xd8 / 4]))(controller, nullptr);

        if (activeObjectAddress != 0)
        {
            // The getter can replace the controller's vtable.
            std::memcpy(&controllerVtable, controller, sizeof(controllerVtable));
            void* activeObject = reinterpret_cast<void*>(
                (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    controllerVtable[0xd8 / 4]))(controller, nullptr));

            (reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, activeObject, 0)[0x3c / 4]))(
                    activeObject, nullptr);
        }

        void* mountedObject = CSF_AT(
            void*, CSF_AT(void*, self, 0x08), 0x118);
        (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, mountedObject, 0)[0xb4 / 4]))(
                mountedObject, nullptr, 0);
    }

    auto* sceneEntries =
        reinterpret_cast<CMenuMpGameMode*>(focusOwner->field_004c);
    void* entry = reinterpret_cast<void*>(
        CMenuMpGameMode_GetSceneEntry(
            sceneEntries, nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x10c)));

    if (CSF_AT(int, entry, 0) == 0 &&
        CSF_AT(uint8_t, entry, 0x0c) != 0 &&
        (CSF_AT(int, entry, 0x10) != -1 ||
         CSF_AT(int, entry, 0x14) != -1))
    {
        (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sceneEntries->vtbl[0x4c / 4]))(
                sceneEntries, nullptr,
                CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x10c));
    }

    auto* position = static_cast<uint8_t*>(
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118)) + 28;

    C_008342e8_CopyFocusEntryPosition(
        focusOwner, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x110),
        static_cast<int>(reinterpret_cast<uintptr_t>(position)));

    void* mountedObject = CSF_AT(
        void*, CSF_AT(void*, self, 0x08), 0x118);
    (reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, mountedObject, 0)[0x8c / 4]))(
            mountedObject, nullptr, 0, 0, -1, 0);

    void* updateObject = static_cast<uint8_t*>(
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118)) + 4;
    return hook::tail_jump(
        updateObject, (*reinterpret_cast<void***>(updateObject))[0x1c / 4]);
}

HOOK_TAIL(0x004A7940, AiMountTasks_ApplyFocusEntryAndUpdate, "ai_mount_tasks");
