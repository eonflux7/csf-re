#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchSceneObjectAction, 0x0049BF00, (void* self, void* edx, int a3));

// Reads a handle from a message record, resolves it in the current scene, and either runs
// the fallback handler or dispatches the action. Returns the handle value when the slot index is negative.
static int __stdcall ScriptCommands_ProcessHandleActionMessage(int a1, int a2, uint32_t* a3)
{
    uint8_t* messageRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
    uint8_t* recordBody = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, messageRecord, 4)));
    int slotIndex = CSF_AT(int, recordBody, 0xC);
    int resultHandle = static_cast<int>(reinterpret_cast<uintptr_t>(recordBody + 4));

    if (slotIndex >= 0)
    {
        int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, resultHandle);
        uint8_t* targetObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(resolvedHandle)));

        if (!targetObject
            || !static_cast<uint8_t>(IsStateCodeAllowed(CSF_AT(uint32_t, targetObject, 0x50)))
            || CSF_AT(int, targetObject, 0x50) != 0
            || sub_485A20(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, targetObject, 0x44))), nullptr, 0x1C) == -1)
        {
            return HandleTargetUnavailable(targetObject, nullptr);
        }

        uint8_t* ownerRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
        uint8_t* ownerBody = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ownerRecord, 4)));
        uint32_t ownerValue = CSF_AT(uint32_t, ownerBody, 4);
        return DispatchSceneObjectAction(targetObject, nullptr, static_cast<int>(ownerValue));
    }

    return resultHandle;
}

HOOK(0x00461660, ScriptCommands_ProcessHandleActionMessage, "auto");
