#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0082e520 base constructor, __fastcall (this in ecx, edx unused), no stack args.
CSF_FUNC(uint32_t*, __fastcall, C_0082e520_Initialize, 0x0041E0F0, (C_0082e520* self, void* /*edx*/));

// Constructs C_0082de40: runs the base initializer, installs this class's vtable,
// and zeroes the state fields (field_001c starts at 10).
// __thiscall, ret 0.
static uint32_t* __fastcall Base_0082de40_Construct(C_0082de40* self, void* /*edx*/)
{
    C_0082e520_Initialize(self, nullptr);

    *(void***)self = CSF_VTBL(0x0082DE40);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040F720, Base_0082de40_Construct, "auto");
