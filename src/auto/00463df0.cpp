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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// 0x24-byte command block built on the stack by the original (C_0082d448 layout).
struct SceneCommandBlock {
    void*    vtbl;        // +0x00  CSF_VTBL(0x0082D448)
    int32_t  handleA;     // +0x04  -1
    int32_t  handleB;     // +0x08  -1
    uint8_t  flagByte;    // +0x0C  0
    int32_t  field10;     // +0x10  0
    int32_t  field14;     // +0x14  0
    int32_t  field18;     // +0x18  0
    float    scale;       // +0x1C  -1.0f
    int32_t  commandKind; // +0x20  7 or 8
};

static int __stdcall SceneDispatchHandleCommand(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // a3 points at a slot holding a record pointer; record+4 holds a node whose
    // +12 is a signed index and whose +4 address is the handle value.
    int recordAddr = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), 0);
    int v3 = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(recordAddr)), 4);
    int v4 = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(v3)), 12);
    int result = v3 + 4;

    if (v4 >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            if (CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x50) == 26)
            {
                void* targetObject = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x34);
                uint32_t targetFlags = CSF_AT(uint32_t, targetObject, 0x44);

                SceneCommandBlock block;
                block.vtbl = CSF_VTBL(0x0082D448);
                block.handleA = -1;
                block.handleB = -1;
                block.flagByte = 0;
                block.field10 = 0;
                block.field14 = 0;
                block.field18 = 0;
                block.scale = -1.0f;
                block.commandKind = ((targetFlags & 2) != 0) + 7;

                // Virtual slot 68 bytes -> index 17.
                using DispatchFn = int(__fastcall*)(void* self, void* edx, void* block);
                DispatchFn dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, targetObject, 0)[17]);
                return dispatch(targetObject, nullptr, &block);
            }
        }
    }
    return result;
}

HOOK(0x463DF0, SceneDispatchHandleCommand, "auto");
