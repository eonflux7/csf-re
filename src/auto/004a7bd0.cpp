#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00834fa8_RestoreCharacterActionAndLookupTask(
    C_00834fa8* self, void* /*edx*/)
{
    void* mount = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_000c) + 256)));

    if (mount)
    {
        CMenuMpGameMode* sceneEntries =
            CSF_AT(CMenuMpGameMode*, mount, 0x4C);
        void* entry = reinterpret_cast<void*>(CMenuMpGameMode_GetSceneEntry(
            sceneEntries, nullptr,
            CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x10C)));

        if (CSF_AT(int, entry, 0) == 0)
        {
            if (CSF_AT(uint8_t, entry, 0x0C))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    sceneEntries->vtbl[0x4C / 4])(
                    sceneEntries, nullptr,
                    CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x10C));
            }

            void* flags = reinterpret_cast<void*>(
                C_008342e8_GetControlFlagsAddress(
                    mount, nullptr,
                    CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x110)));

            if ((CSF_AT(uint8_t, flags, 0) & 1) != 0)
            {
                void* character = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_0008), 0x118);
                void* controller = CSF_AT(void*, character, 0x44);

                if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0xD8 / 4])(
                        controller, nullptr))
                {
                    void* controlledObject =
                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, controller, 0)[0xD8 / 4])(
                            controller, nullptr);

                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, controlledObject, 0)[0x38 / 4])(
                        controlledObject, nullptr, 0);
                }
            }
        }

        void* characterData = reinterpret_cast<void*>(self->field_0008);
        int actionMode = CSF_AT(int, characterData, 0x114);
        Character_SetActionMode(
            CSF_AT(void*, characterData, 0x118), nullptr, actionMode);

        void* character = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0008), 0x118);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, character, 0)[0xB4 / 4])(
            character, nullptr, 0);
    }

    return CharTask_LookupByOffset(
        self, nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_000c) + 236));
}

HOOK(0x004A7BD0, C_00834fa8_RestoreCharacterActionAndLookupTask, "auto");
