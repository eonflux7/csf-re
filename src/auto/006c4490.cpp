#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_00860848_Construct(C_00860848* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00860848);

    self->field_0024 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    self->field_0028 = 1;

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 1;
    self->field_002c = 0;

    void* childMem = Mem_Alloc(28);
    std::uint32_t childAddr = reinterpret_cast<std::uint32_t>(childMem);
    if (childAddr != 0)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(childMem), nullptr);
        *reinterpret_cast<void***>(childMem) = CSF_VTBL(0x0086085C);
        C_00860894_Init(static_cast<std::uint8_t*>(childMem) + 8, nullptr, -1, 0);
        CSF_AT(std::uint32_t, childMem, 4) = reinterpret_cast<std::uint32_t>(self);
        self->field_0030 = childAddr;
    }
    else
    {
        self->field_0030 = 0;
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x006C4490, C_00860848_Construct, "auto");
