#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, IsSceneSlotActive, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsStateFlagSet, 0x00760F90, (int a1));
CSF_FUNC(int, __fastcall, CharActionFsm_LookupAction, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CharActionFsm_ExecuteAction, 0x00485080, (void* self, void* edx, uint32_t a2));

// Typed views of the vtable slots used below (__thiscall callees called through __fastcall pointers).
using ThiscallIntArgFn = int(__fastcall*)(void* self, void* edx, int arg);
using ThiscallVoidArgFn = void(__fastcall*)(void* self, void* edx, int arg);
using ThiscallNoArgBoolFn = uint8_t(__fastcall*)(void* self, void* edx);
using ThiscallNoArgVoidFn = void(__fastcall*)(void* self, void* edx);
using ThiscallFourArgFn = int(__fastcall*)(void* self, void* edx, int a1, int a2, int a3, int a4);

static int __stdcall DispatchSceneObjectCommand(int /*a3*/, int /*a4*/, int* a5)
{
    // Request chain: *a5 -> record; record+4 -> owner; owner+12 = owner index (signed).
    uint8_t* requestRecord = CSF_AT(uint8_t*, a5, 0);
    uint8_t* ownerRecord = CSF_AT(uint8_t*, requestRecord, 4);
    int ownerIndex = CSF_AT(int, ownerRecord, 12);
    int handle = static_cast<int>(reinterpret_cast<uintptr_t>(ownerRecord + 4));

    int result = handle;
    if (ownerIndex < 0)
        return result;

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (result == 0)
        return result;
    uint8_t* entity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));

    result = IsSceneSlotActive(CSF_AT(uint32_t, entity, 0x50));
    if (static_cast<uint8_t>(result) == 0)
        return result;

    // Command bus lookup and dispatch of the command code taken from the second reference.
    uint8_t* valueBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* commandBus = CSF_AT(void*, valueBase, 0x1FC0);
    uint8_t* secondRef = CSF_AT(uint8_t*, a5, 4);
    uint8_t* secondTarget = CSF_AT(uint8_t*, secondRef, 4);
    int commandCode = CSF_AT(int, secondTarget, 4);
    result = (reinterpret_cast<ThiscallIntArgFn>(CSF_AT(void**, commandBus, 0)[3]))(commandBus, nullptr, commandCode);

    int actionIndex = result;
    if (actionIndex == -1)
        return result;

    // Take the state-machine path when the entity has a nonzero field at +0x50,
    // or when the command bus descriptor's flag at +0x74 is clear.
    bool takeFsmStatePath = (CSF_AT(uint32_t, entity, 0x50) != 0);
    if (!takeFsmStatePath)
    {
        uint8_t* valueBaseAgain = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
        void* fsmBus = CSF_AT(void*, valueBaseAgain, 0x1FC0);
        int descriptorValue = (reinterpret_cast<ThiscallIntArgFn>(CSF_AT(void**, fsmBus, 0)[2]))(fsmBus, nullptr, actionIndex);
        uint8_t* descriptor = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(descriptorValue));
        int descriptorFlagArg = CSF_AT(int, descriptor, 0x74);
        takeFsmStatePath = (static_cast<uint8_t>(IsStateFlagSet(descriptorFlagArg)) == 0);
    }

    if (takeFsmStatePath)
    {
        void* stateMachine = CSF_AT(void*, entity, 0x44);
        result = (reinterpret_cast<ThiscallIntArgFn>(CSF_AT(void**, stateMachine, 0)[48]))(stateMachine, nullptr, actionIndex);

        int stateIndex = result;
        if (stateIndex == -1)
            return result;

        if (!(reinterpret_cast<ThiscallNoArgBoolFn>(CSF_AT(void**, stateMachine, 0)[36]))(stateMachine, nullptr))
            (reinterpret_cast<ThiscallNoArgVoidFn>(CSF_AT(void**, stateMachine, 0)[35]))(stateMachine, nullptr);

        (reinterpret_cast<ThiscallVoidArgFn>(CSF_AT(void**, stateMachine, 0)[62]))(stateMachine, nullptr, stateIndex);

        return (reinterpret_cast<ThiscallFourArgFn>(CSF_AT(void**, entity, 0)[35]))(entity, nullptr, 0, 0, -1, 0);
    }

    void* actionFsm = CSF_AT(void*, entity, 0x44);
    result = CharActionFsm_LookupAction(actionFsm, nullptr, actionIndex);
    if (result == -1)
        return result;

    CharActionFsm_ExecuteAction(actionFsm, nullptr, result);
    return result;
}

HOOK(0x00460AA0, DispatchSceneObjectCommand, "auto");
