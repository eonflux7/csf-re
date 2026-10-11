#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Constructs C_008637d8 from a six-dword source array (__thiscall, one stack arg, ret 4).
static C_008637d8* __fastcall C_008637d8_ConstructFromValues(C_008637d8* self, void* /*edx*/, uint32_t* sourceValues) {
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x0082D428));
    *(void***)self = CSF_VTBL(0x008637D8);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x008637BC));
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0020 = reinterpret_cast<uintptr_t>(RwMatrixCreate());
    self->field_0024 = 1;
    self->field_0008 = sourceValues[0];
    self->field_000c = sourceValues[1];
    self->field_0010 = sourceValues[2];
    self->field_0014 = sourceValues[3];
    self->field_0018 = sourceValues[4];
    self->field_001c = sourceValues[5];
    return self;
}

HOOK(0x0070C300, C_008637d8_ConstructFromValues, "auto");
