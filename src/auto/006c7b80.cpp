#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Releases every non-null entry of the pointer array at +0x04 (calling its
// slot-1 virtual with arg 1), then frees the +0x14 and +0x04 buffers and
// re-runs the base-class constructor on this object.
static void __fastcall C_008608f8_ReleaseChildrenAndFreeBuffers(C_008608f8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008608F8);

    uint32_t slotIndex = 0;
    if (self->field_000c != 0)
    {
        do
        {
            uint32_t* slots = reinterpret_cast<uint32_t*>(self->field_0004);
            uint32_t childAddress = slots[slotIndex];
            if (childAddress != 0)
            {
                void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childAddress));
                // Virtual slot +0x4 (vtbl index 1), argument 1.
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
                reinterpret_cast<uint32_t*>(self->field_0004)[slotIndex] = 0;
            }
            ++slotIndex;
        } while (slotIndex < self->field_000c);
    }

    if (self->field_0014 != 0)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014)));
        self->field_0014 = 0;
    }

    if (self->field_0004 != 0)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004)));
        self->field_0004 = 0;
    }

    // Original tail-jumps to the base constructor with ecx = this and no stack args.
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x6C7B80, C_008608f8_ReleaseChildrenAndFreeBuffers, "auto");
