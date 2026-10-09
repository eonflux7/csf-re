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

// Callee: looks up a scene control by id/key in the current scene.
CSF_FUNC(int, __fastcall, sub_452FD0, 0x00452FD0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Raw 32-bit address helpers for the int-typed stack arguments.
static uint8_t* SceneBytePtr(int address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(address)));
}

static int ReadIntAt(int address, int offset)
{
    return *reinterpret_cast<int*>(SceneBytePtr(address) + offset);
}

static void WriteIntAt(int address, int offset, int value)
{
    *reinterpret_cast<int*>(SceneBytePtr(address) + offset) = value;
}

static int __stdcall CopySceneControlBounds(int outputHolder, int unusedArg, int handleRef)
{
    // handleRef -> pointer to a record whose +4 is the node we work from.
    int recordPtr = ReadIntAt(handleRef, 0);
    int nodeV3 = ReadIntAt(recordPtr, 4);
    int signedFlag = ReadIntAt(nodeV3, 12);
    int lookupKey = nodeV3 + 4;
    int result = lookupKey;

    if (signedFlag >= 0)
    {
        result = sub_452FD0(g_pCurrentScene, nullptr, lookupKey);
        if (result != 0)
        {
            int destBase = ReadIntAt(outputHolder, 4) + 4;
            WriteIntAt(destBase, 4, ReadIntAt(result, 32));
            result = ReadIntAt(result, 36);
            WriteIntAt(destBase, 8, result);
        }
    }

    return result;
}

HOOK(0x0045BEF0, CopySceneControlBounds, "auto");
