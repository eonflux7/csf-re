#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns one of the three dword slots stored at this+4, this+8, this+0xC,
// selected by the index argument. Any other index yields 0.
static int __fastcall NetCallbacks_GetSelectionValueByIndex(void* self, void* /*edx*/, int selector) {
    switch (selector) {
    case 0:
        return CSF_AT(int, self, 0x0C);
    case 1:
        return CSF_AT(int, self, 0x08);
    case 2:
        return CSF_AT(int, self, 0x04);
    }
    return 0;
}

HOOK(0x744580, NetCallbacks_GetSelectionValueByIndex, "auto");
