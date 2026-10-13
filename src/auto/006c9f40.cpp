#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void* edx));

static void __fastcall C_00860a30_ReleaseParentState(C_00860a30* self, void* /*edx*/)
{
    uint32_t parentStateHandle = self->field_001c;

    CSF_AT(void**, self, 0) = CSF_VTBL(0x00860A30);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860A24));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860A08));

    if (parentStateHandle) {
        // Virtual slot 1 (byte offset +0x4) on the released object, with argument 1.
        void** parentVtbl = CSF_AT(void**, parentStateHandle, 0);
        ((void(__fastcall*)(void*, void*, int))parentVtbl[1])(
            reinterpret_cast<void*>(parentStateHandle), nullptr, 1);
        self->field_001c = 0;
    }

    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00860990);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860988));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));

    // Tail jump to C_00860170_Construct with `this` in ecx.
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006C9F40, C_00860a30_ReleaseParentState, "auto");
