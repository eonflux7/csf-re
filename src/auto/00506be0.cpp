#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bindings for the original callees (addresses from the packet).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_50BA50, 0x0050BA50, (void* self, void* edx));

// Returns the options-file object stored at self+0x18, creating it on first use.
// If creation fails, the slot is cleared and 0 is returned.
static int __fastcall AppMain_GetOrCreateOptionsFile(void* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x18);
    if (result == 0) {
        void* block = Mem_Alloc(80);
        if (block != nullptr) {
            result = sub_50BA50(block, nullptr);
        } else {
            result = 0;
        }
    }
    CSF_AT(int, self, 0x18) = result;
    return result;
}

HOOK(0x00506BE0, AppMain_GetOrCreateOptionsFile, "auto");
