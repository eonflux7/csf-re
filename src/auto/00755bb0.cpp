#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the embedded sub-object at this+8 when index == -1,
// otherwise the record at (base table + 80 * index), where base table is
// this->field_0004->field_0004.
static int __fastcall C_00869cd0_GetRecordAddress(C_00869cd0* self, void* /*edx*/, int index) {
    if (index == -1) {
        return static_cast<int>(reinterpret_cast<uintptr_t>(self) + 8);
    }
    // self->field_0004 holds a pointer to a table header; the table pointer lives at header+4.
    uint8_t* header = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x4));
    uint32_t tableBase = CSF_AT(uint32_t, header, 0x4);
    return static_cast<int>(tableBase + 80u * static_cast<uint32_t>(index));
}

HOOK(0x00755BB0, C_00869cd0_GetRecordAddress, "auto");
