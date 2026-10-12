#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Zeroes the six dwords of a 0x29+ byte frame, copies three dwords from each
// of two source triples into slots 0..2 and 3..5, and sets the byte at +0x28.
// Returns a pointer to the frame's first dword.
static uint32_t* __fastcall S_00526d40_BuildContactFrame(S_00526d40* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    self->field_0000 = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    self->field_0000 = a2[0];
    CSF_AT(uint32_t, self, 0x04) = a2[1];
    CSF_AT(uint32_t, self, 0x08) = a2[2];

    CSF_AT(uint32_t, self, 0x0C) = a3[0];
    CSF_AT(uint32_t, self, 0x10) = a3[1];
    CSF_AT(uint32_t, self, 0x14) = a3[2];

    CSF_AT(uint8_t, self, 0x28) = 1;

    return &self->field_0000;
}

HOOK(0x004020A0, S_00526d40_BuildContactFrame, "auto");
