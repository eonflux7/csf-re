#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Assigns a C string into the embedded string member at this+0x14 (AssignStringRange).
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

// Constructs the HUD alert icon object: sets vtable, default ids/flags, and copies the icon name.
static C_00831278* __fastcall C_00831278_Construct(C_00831278* self, void* /*edx*/, char* iconName)
{
    self->vtbl = CSF_VTBL(0x00831278);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;

    uint8_t* stringMember = (uint8_t*)self + 0x14;
    CSF_AT(uint32_t, stringMember, 0x18) = 15;
    CSF_AT(uint32_t, stringMember, 0x14) = 0;
    CSF_AT(uint8_t, stringMember, 0x04) = 0;

    AssignStringRange_429B60(stringMember, nullptr, iconName, (unsigned int)strlen(iconName));
    return self;
}

HOOK(0x004A10A0, C_00831278_Construct, "auto");
