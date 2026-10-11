#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_44D9B0, 0x0044D9B0, (void* self, void* edx, unsigned int a2));

// Referenced by the original (call 0x41ED72 site): S_0041e5f0's vtable address.
// Not stored by this function; kept as a file-local binding so the address is present.
static void* const s_GrowButtonArrayVtable = CSF_VTBL(0x0041E5F0);

// Clears the pending-state fields of this object, looks up the current scene's
// task-state slot for the id stored at +0x14, and when found appends that id to the
// scene's list (growing it when full). Returns the new list count, or -1 if not found.
static int __fastcall AiTaskStates_ResetAndQueueId(void* self, void* /*edx*/)
{
    const uint32_t taskStateId = CSF_AT(uint32_t, self, 0x14);

    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint8_t, self, 0x01) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;

    C_008311f0* taskStateTable = CSF_AT(C_008311f0*, g_pCurrentScene, 0x130);
    int result = sub_44D9B0(taskStateTable, nullptr, taskStateId);

    if (result != -1)
    {
        const uint32_t count = taskStateTable->field_001c;
        const uint32_t capacity = CSF_AT(uint32_t, taskStateTable, 0x18);
        S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&taskStateTable->field_0014);

        if (count >= capacity)
            GrowButtonArray(list, nullptr, list->field_000c);

        const uint32_t oldCount = list->field_0008;
        uint32_t* slot = reinterpret_cast<uint32_t*>(list->field_0000 + 4 * oldCount);
        result = static_cast<int>(oldCount + 1);
        list->field_0008 = static_cast<uint32_t>(result);
        *slot = taskStateId;
    }

    CSF_AT(uint32_t, self, 0x14) = 0;
    return result;
}

HOOK(0x0041ED30, AiTaskStates_ResetAndQueueId, "auto");
