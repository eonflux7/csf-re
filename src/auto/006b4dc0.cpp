#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Walks an array of 8-byte entries from startAddr up to endAddr (exclusive).
// Each entry: dword matrix pointer at +0, flag byte at +4. When the flag is
// set, the matrix is destroyed. Returns the last value assigned to the result
// (the flag byte, or the RwMatrixDestroy return truncated to char).
static char __stdcall GfxSkin_ReleaseBufferElements(int startAddr, int endAddr) {
    char result = 0;
    for (std::uint32_t entry = static_cast<std::uint32_t>(startAddr);
         entry != static_cast<std::uint32_t>(endAddr);
         entry += 8) {
        auto* entryPtr = reinterpret_cast<std::uint8_t*>(entry);
        result = static_cast<char>(CSF_AT(std::uint8_t, entryPtr, 4));
        if (result) {
            std::uint32_t* matrix = reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, entryPtr, 0)));
            result = static_cast<char>(RwMatrixDestroy(matrix));
        }
    }
    return result;
}

HOOK(0x006B4DC0, GfxSkin_ReleaseBufferElements, "auto");
