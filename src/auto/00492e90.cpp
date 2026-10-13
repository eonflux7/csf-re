#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00833908_ConfigureFocusEntry, 0x0048E610, (C_00833908* self, void* , int* entry));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, QueryLeafState, 0x006B6D70, (void* self, void* edx, float* a2, char* a3));

// Initializes the player character's base state: configures the focus entry, forwards the
// requested position to the focus target (when it has no type tag), queries the current
// scene leaf state and hands the resulting state byte to the virtual InitStateFlag slot.
static int __fastcall C_00833b88_InitBase(C_00833b88* self, void* /*edx*/, int* a2)
{
    C_00833908_ConfigureFocusEntry(self, nullptr, a2);

    // Focus target: if it exists and its type tag check (vtbl slot 0x1C) returns 0,
    // push the position copied from a2 into its slot 0x50 handler.
    C_0082dd78* focusTarget = reinterpret_cast<C_0082dd78*>(self->field_004c);
    if (focusTarget &&
        !reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, focusTarget, 0)[0x1C / 4])(focusTarget, nullptr))
    {
        uint32_t positionDwords[3];
        positionDwords[0] = static_cast<uint32_t>(a2[0]);
        positionDwords[1] = static_cast<uint32_t>(a2[1]);
        positionDwords[2] = static_cast<uint32_t>(a2[2]);

        C_0082dd78* focusTargetAgain = reinterpret_cast<C_0082dd78*>(self->field_004c);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, focusTargetAgain, 0)[0x50 / 4])(focusTargetAgain, nullptr, positionDwords);
    }

    // Stack slot that carries the state byte. The original overwrites the low byte with 0xFF,
    // QueryLeafState writes its char result into it, and the full dword is then passed on.
    uint32_t stateSlot = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a2));
    reinterpret_cast<uint8_t*>(&stateSlot)[0] = 0xFF;

    void* globalInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    int sceneController = SceneLoader_GetSceneController(globalInt, nullptr);

    void* sceneObj = reinterpret_cast<void*>(static_cast<uintptr_t>(sceneController));
    int sceneStateValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, sceneObj, 0)[0x44 / 4])(sceneObj, nullptr);
    S_006b6be0* leafStateOwner = reinterpret_cast<S_006b6be0*>(static_cast<uintptr_t>(sceneStateValue));

    QueryLeafState(leafStateOwner, nullptr, reinterpret_cast<float*>(a2), reinterpret_cast<char*>(&stateSlot));

    return self->vSlot2C(static_cast<int>(stateSlot));
}

HOOK(0x00492E90, C_00833b88_InitBase, "auto");
