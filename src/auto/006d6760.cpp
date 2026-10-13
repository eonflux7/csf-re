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

CSF_FUNC(char, __fastcall, C_00863018_ReleasePendingStream, 0x006FE820, (C_00863018* self, void*));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));
CSF_FUNC(void, __cdecl, sub_7E4970, 0x007E4970, (int childCount));
CSF_FUNC(int, __cdecl, RtDictRemoveEntry, 0x007FDA80, (int a1, int a2));
CSF_FUNC(int, __cdecl, Rt2dObjectDestroy, 0x007F7900, (int a1));
CSF_FUNC(int, __cdecl, _rtBitMatrixDestroy, 0x007FDB40, (int a1));
CSF_FUNC(int, __cdecl, Rt2dMaestroDestroy, 0x007FD9C0, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the render handle of a Rt2d scene: removes and destroys every
// registered child object, frees the entry table and the bit matrix, destroys
// the scene itself, and clears the pending-stream flag.
static int __fastcall GfxScene_ResetRenderHandle(C_008610c0* self, void* /*edx*/)
{
    C_00863018_ReleasePendingStream(
        reinterpret_cast<C_00863018*>(reinterpret_cast<uint8_t*>(self) + 0x74), nullptr);

    int result = static_cast<int>(self->field_003c);
    uint32_t entryIndex = 0;
    if (result)
    {
        int childCount = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
        sub_7E4970(childCount);

        if (self->field_0048)
        {
            do
            {
                // Entries are 8-byte records; the object handle is the second dword.
                int objectHandle = *reinterpret_cast<int*>(self->field_0040 + 8 * entryIndex + 4);
                RtDictRemoveEntry(static_cast<int>(self->field_003c), objectHandle);
                Rt2dObjectDestroy(objectHandle);
                ++entryIndex;
            } while (entryIndex < self->field_0048);
            entryIndex = 0;
        }

        uint32_t entryTable = self->field_0040;
        self->field_0044 = 0;
        self->field_0048 = 0;
        if (entryTable)
        {
            Mem_Free(reinterpret_cast<void*>(entryTable));
            self->field_0040 = 0;
        }

        _rtBitMatrixDestroy(static_cast<int>(self->field_0050));

        uint32_t* sceneHandle = reinterpret_cast<uint32_t*>(self->field_003c);
        self->field_0050 = 0;
        // The original pushes only the scene handle; the second argument is a
        // stack leftover there, so it is passed as 0.
        result = Rt2dMaestroDestroy(sceneHandle, 0);
        self->field_003c = 0;
    }

    self->field_00d9 = 0;
    return result;
}

HOOK(0x006D6760, GfxScene_ResetRenderHandle, "auto");
