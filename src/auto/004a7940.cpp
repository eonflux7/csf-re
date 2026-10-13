#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, CopyFocusEntryPosition, 0x0049EE00, (void* self, void* edx, int arg0, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static hook::Tail __fastcall AiMountTasks_ApplyFocusEntryAndUpdate(
    void* self, const uint32_t* /*args*/)
{
    auto* focusOwner = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(
                static_cast<uint8_t*>(CSF_AT(void*, self, 0x0C)) + 0x100)));

    if (!focusOwner)
        return hook::tail_ret(0);

    auto* flags = reinterpret_cast<uint8_t*>(
        C_008342e8_GetControlFlagsAddress(
            focusOwner, nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110)));

    if ((*flags & 1) != 0)
    {
        void* controller = CSF_AT(
            void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), 0x44);

        if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0xD8 / 4])(controller, nullptr))
        {
            void* activeObject =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controller, 0)[0xD8 / 4])(
                    controller, nullptr);

            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, activeObject, 0)[0x3C / 4])(
                activeObject, nullptr);
        }

        void* mountedObject =
            CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, mountedObject, 0)[0xB4 / 4])(
            mountedObject, nullptr, 0);
    }

    auto* sceneEntries =
        reinterpret_cast<CMenuMpGameMode*>(focusOwner->field_004c);
    void* entry = reinterpret_cast<void*>(
        CMenuMpGameMode_GetSceneEntry(
            sceneEntries, nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C)));

    if (CSF_AT(int, entry, 0) == 0 &&
        CSF_AT(uint8_t, entry, 0x0C) != 0 &&
        (CSF_AT(int, entry, 0x10) != -1 ||
         CSF_AT(int, entry, 0x14) != -1))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sceneEntries->vtbl[0x4C / 4])(
            sceneEntries, nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C));
    }

    void* mountedObject =
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
    CopyFocusEntryPosition(
        focusOwner, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110),
        reinterpret_cast<int>(static_cast<uint8_t*>(mountedObject) + 0x1C));

    mountedObject = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, mountedObject, 0)[0x8C / 4])(
        mountedObject, nullptr, 0, 0, -1, 0);

    mountedObject = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
    void* updateObject = static_cast<uint8_t*>(mountedObject) + 4;
    return hook::tail_jump(
        updateObject, (*reinterpret_cast<void***>(updateObject))[0x1C / 4]);
}

HOOK_TAIL(0x004A7940, AiMountTasks_ApplyFocusEntryAndUpdate, "ai_mount_tasks");
