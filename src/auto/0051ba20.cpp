#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns how many 68-byte records lie between the begin and end pointers of the
// list held by this object. Returns 0 when the begin pointer is null.
static int __fastcall C_00840e38_GetRecordCount(C_00840e38* self, void* /*edx*/) {
    S_0075b5e0* recordList = reinterpret_cast<S_0075b5e0*>(self->field_0004);
    uint32_t beginAddress = CSF_AT(uint32_t, recordList, 0x4);
    if (beginAddress == 0) {
        return 0;
    }
    uint32_t endAddress = CSF_AT(uint32_t, recordList, 0x8);
    int32_t byteSpan = static_cast<int32_t>(endAddress - beginAddress);
    return byteSpan / 68;
}

HOOK(0x0051BA20, C_00840e38_GetRecordCount, "auto");
