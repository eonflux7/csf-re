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

// Callee at 0x006CE1E0: thiscall with one stack argument (ret 4).
CSF_FUNC(char*, __fastcall, CollisionShape_LoadFromSource, 0x006CE1E0, (void* self, void* edx, int a2));

// Copies the 32-bit value at byte offset 0x60 from the source object into self.
// The callee's result is discarded, as in the original.
static int __stdcall CollisionShapes_CopyFromSource(int a1, uint32_t* a2)
{
    int result = 0;

    if (a1)
    {
        void* self = reinterpret_cast<void*>(a1);
        CollisionShape_LoadFromSource(self, nullptr, reinterpret_cast<int>(a2));
        result = (int)CSF_AT(uint32_t, a2, 0x60);
        CSF_AT(int, self, 0x60) = result;
    }
    return result;
}

HOOK(0x006CE2E0, CollisionShapes_CopyFromSource, "auto");
