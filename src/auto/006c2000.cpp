#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Zeroes the record's header dword and the eight dwords that follow it
// (offsets 0x00..0x20), then returns a pointer to the header dword.
static uint32_t* __fastcall TexDictRecord_Construct(S_006c2000* self, void* /*edx*/)
{
    self->field_0000 = 0;

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;

    return &self->field_0000;
}

HOOK(0x006C2000, TexDictRecord_Construct, "auto");
