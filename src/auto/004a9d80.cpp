#include <cstdint>
#include "globals.h"

#include "hook.h"

// Sets the five-dword state block to its initial values (-1, -1, 0, 0, 0)
// and returns the object pointer.
static uint32_t* __fastcall AiCore_ConstructStateData(uint32_t* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return self;
}

HOOK(0x4A9D80, AiCore_ConstructStateData, "auto");
