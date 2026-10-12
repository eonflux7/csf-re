#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds the callee at 0x4E09B0: a __thiscall method (ecx = item, ret 16) that
// expands the four bounds (float*) with the item's extents.
CSF_FUNC(unsigned int, __fastcall, S_004e09b0_ExpandBounds, 0x004E09B0,
         (void* self, void* edx, float* a2, float* a3, float* a4, float* a5));

// Resets the bounding rectangle of this struct to the "empty" sentinel values, then
// walks an array of 64-byte items (base at field_0024, count at field_0020) and
// expands the rectangle with each item. Returns the item count as read after the
// last iteration.
static unsigned int __fastcall S_004ddf20_AccumulateBounds(S_004ddf20* self, void* /*edx*/)
{
    unsigned int count = self->field_0020;
    uint32_t itemBase = self->field_0024;

    unsigned int index = 0;

    self->field_000c = 100000000.0f;
    self->field_0010 = -100000000.0f;
    self->field_0014 = 100000000.0f;
    self->field_0018 = -100000000.0f;

    while (index < count)
    {
        S_004e09b0_ExpandBounds(reinterpret_cast<void*>(itemBase), nullptr,
                                &self->field_000c, &self->field_0014,
                                &self->field_0010, &self->field_0018);
        count = self->field_0020;
        ++index;
        itemBase += 64;
    }
    return count;
}

HOOK(0x004DDF20, S_004ddf20_AccumulateBounds, "auto");
