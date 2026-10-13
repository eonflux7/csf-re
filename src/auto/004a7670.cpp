#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static C_008342e8* __fastcall C_00834fa8_SelectMountTransition(
    C_00834fa8* self, void* /*edx*/)
{
    auto* control = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 256)));
    if (!control)
        return control;

    auto* scene = reinterpret_cast<CMenuMpGameMode*>(control->field_004c);
    alignas(8) uint8_t positionBytes[0xC];
    auto* position = reinterpret_cast<uint32_t*>(positionBytes);

    C_008342e8_CopyControlPosition(
        control, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0C), 272),
        position);

    auto* sceneEntry = reinterpret_cast<uint32_t*>(
        CMenuMpGameMode_GetSceneEntry(
            scene, nullptr, static_cast<int>(position[0])));

    void* owner = CSF_AT(void*, self, 0x08);
    CSF_AT(uint32_t, owner, 276) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 280), 480);

    if (*sceneEntry)
    {
        void* character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 280);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, character, 0)[180 / 4])(
                character, nullptr, 1);

        Character_SetActionMode(
            CSF_AT(void*, CSF_AT(void*, self, 0x08), 280),
            nullptr, 0);

        return reinterpret_cast<C_008342e8*>(
            CharTask_LookupByOffset(
                self, nullptr,
                static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 100)));
    }

    return reinterpret_cast<C_008342e8*>(
        CharTask_LookupByOffset(
            self, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 156)));
}

HOOK(0x004A7670, C_00834fa8_SelectMountTransition, "auto");
