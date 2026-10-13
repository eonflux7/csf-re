#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, RpPrtStd_ReleaseParentState, 0x006C9F40, (void* self, void* edx));

// Resets the object's vtables and embedded pointers, releases the child
// object held at +0x94 (virtual slot 1 with argument 1) if present, then
// releases the parent render state.
static void __fastcall C_00861508_ReleaseChildAndParentState(C_00861508* self, void* /*edx*/) {
    uint32_t childAddress = self->field_0094;

    self->vtbl = CSF_VTBL(0x00861508);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00861500));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008614E4));

    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
        self->field_0094 = 0;
    }

    RpPrtStd_ReleaseParentState(self, nullptr);
}

HOOK(0x6DC910, C_00861508_ReleaseChildAndParentState, "auto");
