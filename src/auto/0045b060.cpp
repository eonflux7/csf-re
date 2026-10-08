#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets this->+0x18 = 12 and this->+0x82 = 0, then follows the chain
// *(*a4 + 4) -> linked, and stores *(linked + 4) into this->+0x78.
// Returns the intermediate "linked" pointer.
static int __fastcall InitSlotFromSourceRecord(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, void* a4)
{
    CSF_AT(uint32_t, self, 0x18) = 12;
    CSF_AT(uint8_t, self, 0x82) = 0;

    void* recordPtr = CSF_AT(void*, a4, 0);
    void* linkedPtr = CSF_AT(void*, recordPtr, 4);
    uint32_t linkedValue = CSF_AT(uint32_t, linkedPtr, 4);

    CSF_AT(uint32_t, self, 0x78) = linkedValue;
    return static_cast<int>(reinterpret_cast<uintptr_t>(linkedPtr));
}

HOOK(0x0045B060, InitSlotFromSourceRecord, "auto");
