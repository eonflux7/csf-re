#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, sub_7028C0, 0x007028C0, (void* self, void* edx));

static int __fastcall C_00863180_Destructor(C_00863180* self, void* /*edx*/, char flags) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863180);

    sub_7028C0(self, nullptr);

    if (self->field_0010) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((flags & 1) != 0) {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x702B10, C_00863180_Destructor, "auto");
