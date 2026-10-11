#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up `key` in the two-entry table at 0x8A85A8, using the row chosen by
// the object's virtual slot 0xA8 (42 dwords into the vtable). Returns the
// column index (0 or 1) on a match, or 0 if no column matches.
static int __fastcall C_00863ba8_FindMatchingEntryIndex(C_00863ba8* self, void* /*edx*/, int key)
{
    int column = 0;
    while (true) {
        // Virtual call through vtable slot 0xA8 (index 42); no arguments.
        int row = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr);

        // Table is an array of dwords: element index = row * 4 + column.
        int entry = CSF_AT(int, CSF_ADDR_DATA(0x008A85A8), (row * 4 + column) * 4);
        if (key == entry) {
            return column;
        }
        ++column;
        if (column >= 2) {
            return 0;
        }
    }
}

HOOK(0x0070F080, C_00863ba8_FindMatchingEntryIndex, "auto");
