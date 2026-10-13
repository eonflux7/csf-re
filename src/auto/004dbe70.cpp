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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases the owned child (slot 0 with arg 1), clears the link, restores the base
// vtable, and frees the block when bit 0 of a2 is set. Returns the object pointer.
static void* __fastcall C_0083def8_Destroy(C_0083def8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0083DEF8);

    void* ownedChild = reinterpret_cast<void*>(self->field_000c);
    if (ownedChild != nullptr)
    {
        using ChildReleaseFn = void(__fastcall*)(void* self, void* edx, int mode);
        ChildReleaseFn releaseSlot0 = reinterpret_cast<ChildReleaseFn>(CSF_AT(void**, ownedChild, 0)[0]);
        releaseSlot0(ownedChild, nullptr, 1);
        self->field_000c = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x004DBE70, C_0083def8_Destroy, "auto");
