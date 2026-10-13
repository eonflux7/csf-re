#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CharActionFsm_TryApplyMissionAction, 0x00486C30, (S_00486c30* self, void* , float a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiGiveObject_ApplyMissionAction(void* self, void* /*edx*/)
{
    // Retain the packet's action-FSM address binding.
    (void)CSF_VTBL(0x00486C30);

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* objectTable = CSF_AT(void*, manager, 0x1FC8);
    void* taskOwner = CSF_AT(void*, self, 0x14);
    void* object = CSF_AT(void*, taskOwner, 0x2C);

    int objectIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, objectTable, 0)[0x10 / 4])(
                objectTable, nullptr, CSF_AT(int, object, 0xC0));
    if (objectIndex == -1)
        return objectIndex;

    manager = reinterpret_cast<void*>(GetValue8B0554());
    objectTable = CSF_AT(void*, manager, 0x1FC8);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, objectTable, 0)[8 / 4])(
                objectTable, nullptr, objectIndex);

    manager = reinterpret_cast<void*>(GetValue8B0554());
    void* actionTable = CSF_AT(void*, manager, 0x1FC0);
    int actionIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, actionTable, 0)[0xC / 4])(
                actionTable, nullptr, CSF_AT(int, definition, 0xC0));
    if (actionIndex == -1)
        return actionIndex;

    int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x3BC));
    if (!resolved)
        return resolved;

    void* character = reinterpret_cast<void*>(resolved);
    S_00486c30* actionFsm = CSF_AT(S_00486c30*, character, 0x44);

    // The original passes the index's raw bits through the float parameter.
    float actionArgument;
    std::memcpy(&actionArgument, &actionIndex, sizeof(actionArgument));

    reinterpret_cast<void(__fastcall*)(S_00486c30*, void*, float, int)>(
        reinterpret_cast<void**>(actionFsm->field_0000)[0xA8 / 4])(
            actionFsm, nullptr, actionArgument, 0);
    CharActionFsm_TryApplyMissionAction(actionFsm, nullptr, actionArgument);

    manager = reinterpret_cast<void*>(GetValue8B0554());
    actionTable = CSF_AT(void*, manager, 0x1FC0);
    void* actionDefinition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, actionTable, 0)[8 / 4])(
                actionTable, nullptr, actionIndex);

    if (CSF_AT(int, actionDefinition, 0x74) == 27)
    {
        void* controller = CSF_AT(void*, character, 0x38);
        void** controllerVtable = CSF_AT(void**, controller, 0);
        int state =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                controllerVtable[0x84 / 4])(controller, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            controllerVtable[0x94 / 4])(controller, nullptr, state);
    }

    Get_008b0678();
    return MenuMissionInfo_SetGlobalIndex(95);
}

HOOK(0x004D4BB0, AiGiveObject_ApplyMissionAction, "auto");
