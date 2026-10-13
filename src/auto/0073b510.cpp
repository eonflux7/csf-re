#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies a 0x24-byte source record (a2) into the object at ecx. The original
// is __thiscall with one stack argument and `ret 4`.
static int __fastcall CopyRecordFieldsFromSource(void* self, void* /*edx*/, void* a2) {
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, a2, 0x04);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, a2, 0x08);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, a2, 0x20);
    uint32_t result = CSF_AT(uint32_t, a2, 0x18);
    CSF_AT(uint32_t, self, 0x20) = result;
    return static_cast<int>(result);
}

HOOK(0x0073B510, CopyRecordFieldsFromSource, "auto");
