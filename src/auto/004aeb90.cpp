#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_004aa4d0_IsWithinMovementThreshold, 0x004AA4D0, (S_004aa4d0* self, void* , float threshold));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, TableContainsKey, 0x004E35A0, (uint32_t* self, void* , int a2));
CSF_FUNC(int, __fastcall, C_0083e9d0_FindHighestEnabledLevel, 0x004E7330, (C_0083e9d0* self, void*));
CSF_FUNC(int, __fastcall, S_004f0cd0_GetActiveGroupIndex, 0x004F15B0, (S_004f0cd0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004aa4d0_GetPathStatus(
    S_004aa4d0* self, void* /*edx*/)
{
    // The packet lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x004AA4D0);

    void* owner = CSF_AT(void*, self, 0x2C);
    void* ownerState = CSF_AT(void*, owner, 0x34);

    if (CSF_AT(uint8_t, ownerState, 0x39) != 0)
    {
        if (CSF_AT(int, self, 0x630) >= 0)
            return CSF_AT(int, self, 0x604);

        int highestEnabledLevel = C_0083e9d0_FindHighestEnabledLevel(
            CSF_AT(C_0083e9d0*, owner, 0x3C), nullptr);

        owner = CSF_AT(void*, self, 0x2C);
        int activeGroupIndex = S_004f0cd0_GetActiveGroupIndex(
            CSF_AT(S_004f0cd0*, owner, 0x40), nullptr);

        if (highestEnabledLevel != -1 &&
            (activeGroupIndex == -1 || highestEnabledLevel > activeGroupIndex))
        {
            return highestEnabledLevel;
        }

        return activeGroupIndex;
    }

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<uint8_t*>(self) + 8, nullptr);
    void* linkedObject = linkedValue != 0
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    void* linkedOwner = CSF_AT(void*, linkedObject, 0x2C);
    void* linkedState = CSF_AT(void*, linkedOwner, 0x34);
    if (CSF_AT(int, linkedState, 0x2F4) != 1)
        return -1;

    owner = CSF_AT(void*, self, 0x2C);
    int movementMode = CSF_AT(int, owner, 0x1D8);
    if (movementMode != 2 && movementMode != 3)
        return -1;

    void* scene = g_pCurrentScene;
    uint32_t* table = CSF_AT(uint32_t*, owner, 0x38);
    uint32_t sceneContext = GetSceneMenuContext(scene, nullptr);
    if (sceneContext == 0)
        return -1;

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (S_004aa4d0_IsWithinMovementThreshold(
            self, nullptr, CSF_AT(float, globalState, 0xABC)) ||
        TableContainsKey(table, nullptr, static_cast<int>(sceneContext + 0x1C)))
    {
        return 3;
    }

    return -1;
}

HOOK(0x004AEB90, S_004aa4d0_GetPathStatus, "auto");
