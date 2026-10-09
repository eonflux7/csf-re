#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at index `index` of the table pointed to by field_0078.
static int __fastcall CCharActionFsm_GetRecordByIndex(CCharActionFsm* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(self->field_0078);
    return table[index];
}

HOOK(0x485940, CCharActionFsm_GetRecordByIndex, "auto");
