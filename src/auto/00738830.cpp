#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slots of the object stored in field_0024 (byte offsets 0x84 and 0x88 -> slots 33 and 34).
static int __fastcall C_00867358_SetAttachedObjectAndReleaseOld(C_00867358* self, void* /*edx*/, int newValue);

static int __fastcall C_00867358_SetAttachedObjectAndReleaseOld(C_00867358* self, void* /*edx*/, int newValue)
{
    int result = newValue;
    const uint32_t oldValue = self->field_0020;
    self->field_0020 = static_cast<uint32_t>(newValue);

    if (newValue != 0) {
        void* attached = reinterpret_cast<void*>(self->field_0024);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, attached, 0)[0x84 / 4])(
            attached, nullptr, newValue);
    }

    if (oldValue != 0) {
        void* attached = reinterpret_cast<void*>(self->field_0024);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, attached, 0)[0x88 / 4])(
            attached, nullptr, static_cast<int>(oldValue));
    }

    return result;
}

HOOK(0x738830, C_00867358_SetAttachedObjectAndReleaseOld, "auto");
