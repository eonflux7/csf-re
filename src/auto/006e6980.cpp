#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_00861ce0_ConstructAtomic(C_00861ce0* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D428));
    self->vtbl = CSF_VTBL(0x00861CE0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00861CD4));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00861CB8));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = -1;
    self->field_0018 = 0;
    self->field_0019 = 1;
    self->field_0028 = 0;
    self->field_002c = -1;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0044 = -1;
    self->field_0048 = 0;

    void* companion = Mem_Alloc(28);
    if (companion)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(companion), nullptr);
        CSF_AT(void**, companion, 0x0) = CSF_VTBL(0x00861C80);
        C_00860894_Init(static_cast<uint8_t*>(companion) + 8, nullptr, -1, 0);
        CSF_AT(uint32_t, companion, 0x4) = reinterpret_cast<uint32_t>(self);
    }

    self->field_0040 = reinterpret_cast<uint32_t>(companion);
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x006E6980, C_00861ce0_ConstructAtomic, "auto");
