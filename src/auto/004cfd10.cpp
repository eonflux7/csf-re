#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_4CFA20, 0x004CFA20, (void* self, void* edx, int a2));

static int __fastcall C_0083b580_InitWithUnitScale(C_0083b580* self, void* /*edx*/, int a2) {
    sub_4CFA20(self, nullptr, a2);

    C_00833908* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->vtbl = CSF_VTBL(0x0083B580);
    CSF_AT(uint32_t, self, 0x4) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083B564));
    self->field_0060 = static_cast<uint32_t>(-1);
    self->field_0064 = static_cast<uint32_t>(-1);

    void* level1 = reinterpret_cast<void*>(owner->vGetField016c());
    void* level2 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, level1, 0)[20])(level1, nullptr);
    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, level2, 0)[1])(level2, nullptr, 1065353216);

    return reinterpret_cast<int>(self);
}

HOOK(0x004CFD10, C_0083b580_InitWithUnitScale, "auto");
