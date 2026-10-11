#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

// Constructor of C_008636fc (water effect object). Takes one stack argument (ret 4).
static int __fastcall C_008636fc_Init(C_008636fc* self, void* /*edx*/, int initialParam)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x008636FC);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_0014 = static_cast<uint32_t>(initialParam);
    self->field_0018 = 0;
    self->field_0020 = 1;

    // Secondary object allocated from the game heap.
    C_00860178* helper = static_cast<C_00860178*>(Mem_Alloc(static_cast<std::size_t>(28)));
    if (helper)
    {
        C_00860178_Construct(helper, nullptr);
        *reinterpret_cast<void***>(helper) = CSF_VTBL(0x0086370C);
        C_00860894_Init(reinterpret_cast<uint8_t*>(helper) + 8, nullptr, -1, 0);
        CSF_AT(C_008636fc*, helper, 4) = self;
        self->field_001c = reinterpret_cast<uint32_t>(helper);
    }
    else
    {
        self->field_001c = 0;
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x0070B6E0, C_008636fc_Init, "auto");
