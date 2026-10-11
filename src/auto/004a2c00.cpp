#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the cached slots of C_008344f0 to their defaults (-1 for "unset" ids,
// 0 otherwise, 0.0f for the float at +0x64). Then asks the owner object at +0x08
// for a child (vSlot28), queries that child (vSlot68), and if it returned a
// non-null handle, forwards a (0, 0) notification to it (vSlot1C).
static int __fastcall C_008344f0_ResetFieldsAndNotify(C_008344f0* self, void* /*edx*/)
{
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0xFFFFFFFFu;
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0xFFFFFFFFu;
    self->field_0028 = 0xFFFFFFFFu;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0xFFFFFFFFu;
    self->field_0038 = 0xFFFFFFFFu;
    self->field_0044 = 0xFFFFFFFFu;
    self->field_0048 = 0xFFFFFFFFu;
    self->field_004c = 0xFFFFFFFFu;
    self->field_0064 = 0.0f;

    // owner = self->field_0008; child = owner->vSlot28()
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    void** ownerVtbl = CSF_AT(void**, owner, 0);
    int child = ((int(__fastcall*)(void*, void*))ownerVtbl[0x28 / 4])(owner, nullptr);
    if (child == 0)
        return 0;

    // hit = child->vSlot68(); if (hit != 0) child->vSlot1C(0, 0)
    void* childObj = reinterpret_cast<void*>(static_cast<uintptr_t>(child));
    void** childVtbl = CSF_AT(void**, childObj, 0);
    int hit = ((int(__fastcall*)(void*, void*))childVtbl[0x68 / 4])(childObj, nullptr);
    if (hit == 0)
        return hit;

    return static_cast<int>(
        ((char(__fastcall*)(void*, void*, int, int))childVtbl[0x1C / 4])(childObj, nullptr, 0, 0));
}

HOOK(0x4A2C00, C_008344f0_ResetFieldsAndNotify, "auto");
