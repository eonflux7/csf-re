#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears the 93-entry table that starts at this+12 (5-dword records, stride 0x14),
// then dispatches through vtable slot 0x40 with this+4 as the argument.
static int __fastcall C_00830030_ClearSlotTableAndDispatch(C_00830030* self, void* /*edx*/) {
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    uint8_t* tableBase = selfBytes + 4;

    if (tableBase != nullptr) {
        uint8_t* cursor = tableBase + 8;
        for (int remaining = 93; remaining != 0; --remaining) {
            CSF_AT(int, cursor, -8) = -1;
            CSF_AT(int, cursor, -4) = -1;
            CSF_AT(int, cursor, 0) = 0;
            CSF_AT(float, cursor, 4) = 1.0f;
            CSF_AT(int, cursor, 8) = 0;
            cursor += 0x14;
        }
    }

    return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0x40 / 4])(
        self, nullptr, tableBase);
}

HOOK(0x428AC0, C_00830030_ClearSlotTableAndDispatch, "auto");
