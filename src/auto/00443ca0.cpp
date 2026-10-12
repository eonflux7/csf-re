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

CSF_FUNC(void, __fastcall, sub_443C50, 0x00443C50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(void, __fastcall, SceneLoader_RemoveObjectEntry, 0x00475480, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the scene subsystem B8 state: clears the sub-object at +0x0C, frees the
// buffer at +0x1C, zeroes the two counters at +0x20/+0x24, then removes this
// object's entry (+0x04) from the global object field 184 registry.
static int __fastcall C_0083109c_ResetSubsystemB8(C_0083109c* self, void* /*edx*/)
{
    sub_443C50(reinterpret_cast<void*>(&self->field_000c), nullptr);

    uint32_t bufferPointer = self->field_001c;
    self->field_0020 = 0;
    self->field_0024 = 0;
    if (bufferPointer)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(bufferPointer)));
        self->field_001c = 0;
    }

    // The registry object type is not declared here; it is only passed through as an opaque pointer.
    void* globalObjectField184 =
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalObjectField184()));
    if (self)
    {
        int entryAddress = static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0004));
        SceneLoader_RemoveObjectEntry(globalObjectField184, nullptr, entryAddress);
    }
    else
    {
        SceneLoader_RemoveObjectEntry(globalObjectField184, nullptr, 0);
    }

    // The original returns eax left over from the last call (uninitialized local); pass 0.
    return 0;
}

HOOK(0x443CA0, C_0083109c_ResetSubsystemB8, "auto");
