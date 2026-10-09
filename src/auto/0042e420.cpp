#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resets a HUD widget's state block to its default values. Layout is not
// declared in types/auto_classes.h, so every field is accessed by byte offset.
static uint32_t* __fastcall HudWidget_InitDefaults(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0x3F800000u; // float 1.0f
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0xFFFFFFFFu; // -1
    return static_cast<uint32_t*>(self);
}

HOOK(0x42E420, HudWidget_InitDefaults, "auto");
