#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833b88_IsInactive, 0x0048E100, (C_00833b88* self, void*));
CSF_FUNC(bool, __stdcall, CoverTask_IsActiveAndLinkedToSlot, 0x004ABE10, (int a1, int a2));
CSF_FUNC(int, __stdcall, AiCoverTasks_IsNotDifferentFromLinkedEntry, 0x004ABE70, (int a1, int a2));
CSF_FUNC(bool, __stdcall, FncSearch_CheckSearchTargetResolved, 0x004B5E70, (int a1, int a2));
CSF_FUNC(int, __cdecl, GameConfig_IsAcceptedOptionId, 0x00755960, (int optionId));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __stdcall AiDistracted_CheckLinkedObjectTarget(int task, int handle)
{
    if (CSF_AT(int, reinterpret_cast<void*>(handle), 8) < 0)
        return false;

    C_00833b88* object = reinterpret_cast<C_00833b88*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (!object)
        return false;

    if (static_cast<std::uint8_t>(
            GameConfig_IsAcceptedOptionId(object->field_0050)) == 0)
        return false;

    if (static_cast<std::uint8_t>(
            C_00833b88_IsInactive(object, nullptr)) != 0)
        return false;

    if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            object->vtbl[0x7C / 4])(object, nullptr))
        return false;

    const int objectAddress = reinterpret_cast<int>(object);
    return CoverTask_IsActiveAndLinkedToSlot(task, objectAddress)
        && static_cast<std::uint8_t>(
               AiCoverTasks_IsNotDifferentFromLinkedEntry(task, objectAddress)) != 0
        && FncSearch_CheckSearchTargetResolved(task, objectAddress);
}

HOOK(0x004BBAF0, AiDistracted_CheckLinkedObjectTarget, "auto");
