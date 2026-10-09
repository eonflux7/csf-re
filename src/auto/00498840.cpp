#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears the contextual-selection state of a CharActionFsm object.
// Fields at 0x218, 0x298, 0x2a0-0x2ac, 0x2cc, 0x2d0 are not declared in
// C_00833908, so they are accessed by byte offset via CSF_AT.
static int __fastcall CharActionFsm_ClearContextualSelection(C_00833908* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x218) = 0x3E6B851Fu;
    CSF_AT(int, self, 0x298) = 0;
    CSF_AT(int, self, 0x2A0) = -1;
    CSF_AT(int, self, 0x2A4) = -1;
    CSF_AT(int, self, 0x2A8) = -1;
    CSF_AT(int, self, 0x2AC) = -1;
    CSF_AT(int, self, 0x2CC) = -1;
    CSF_AT(int, self, 0x2D0) = -1;
    return -1;
}

HOOK(0x00498840, CharActionFsm_ClearContextualSelection, "auto");
