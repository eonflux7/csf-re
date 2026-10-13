#include <cstdint>
#include "globals.h"

#include "hook.h"

// 0x00692B50: __thiscall(this, int index), ret 4.
// Returns 0 when the byte at this+0x14 is set or index == -1;
// otherwise returns the int at index in the array pointed to by this+0x2C.
static int __fastcall CoreParser_ReadIndexedEntry(void* self, void* /*edx*/, int index) {
    if (CSF_AT(uint8_t, self, 0x14) != 0 || index == -1) {
        return 0;
    }
    int* entries = CSF_AT(int*, self, 0x2C);
    return entries[index];
}

HOOK(0x692B50, CoreParser_ReadIndexedEntry, "auto");
