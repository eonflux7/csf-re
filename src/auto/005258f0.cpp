#include <cstdint>
#include <cstddef>

#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bindings for the original functions this one calls.
CSF_FUNC(int, __fastcall, ResetAndReleaseObjectSlots, 0x00525060, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the second scene-manager slot of a C_00841090 object: releases the
// object slot table, calls vtable slot 19 (+0x4C) on the three owned sub-objects
// and clears their pointers, resets the position/index fields to -1 or 0, and
// frees the buffer at +0x8C. Returns the last value left in eax (the Mem_Free
// result when the buffer was present, otherwise the buffer pointer value, 0).
static int __fastcall SceneLoader_ResetSecondManager(C_00841090* self, void* /*edx*/)
{
    ResetAndReleaseObjectSlots(&self->field_0028, nullptr);

    // Owned object at +0x0C: virtual release (slot +0x4C).
    uint32_t ownedAt0C = self->field_000c;
    if (ownedAt0C != 0) {
        void* obj = reinterpret_cast<void*>(ownedAt0C);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x4C / 4])(obj, nullptr);
        self->field_000c = 0;
    }

    // Owned object at +0x08: virtual release (slot +0x4C).
    uint32_t ownedAt08 = self->field_0008;
    if (ownedAt08 != 0) {
        void* obj = reinterpret_cast<void*>(ownedAt08);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x4C / 4])(obj, nullptr);
        self->field_0008 = 0;
    }

    // Owned object at +0x10: virtual release (slot +0x4C).
    uint32_t ownedAt10 = self->field_0010;
    if (ownedAt10 != 0) {
        void* obj = reinterpret_cast<void*>(ownedAt10);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x4C / 4])(obj, nullptr);
        self->field_0010 = 0;
    }

    self->field_0014 = 0;
    self->field_001c = static_cast<uint32_t>(-1);
    self->field_0020 = static_cast<uint32_t>(-1);
    self->field_0024 = static_cast<uint32_t>(-1);
    self->field_0018 = 0;

    uint32_t buffer = self->field_008c;
    int result = static_cast<int>(buffer);
    self->field_0090 = 0;
    self->field_0094 = 0;
    if (buffer != 0) {
        result = Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_008c = 0;
    }
    return result;
}

HOOK(0x005258F0, SceneLoader_ResetSecondManager, "auto");
