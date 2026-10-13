#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// COR_ILMETHOD_SECT::More: the high bit of the section header's first byte
// flags whether another section follows.
static int __fastcall IlMethodSect_HasMoreSections(void* self, void* /*edx*/) {
    uint8_t headerByte = CSF_AT(uint8_t, self, 0);
    return headerByte >> 7;
}

HOOK(0x690B10, IlMethodSect_HasMoreSections, "auto");
