#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys every RwMatrix in a counted, 28-byte-stride array (flag byte at element+12,
// matrix pointer at element+0 after stepping), then frees the block header (count at data-4).
// Returns the value from Mem_Free, or 0 if the array pointer was null.
static int __fastcall RwMatrixArray_DestroyAndFree(int* self, void* /*edx*/)
{
    int result = 0;
    CSF_AT(int, self, 0x4) = 0;
    CSF_AT(int, self, 0x8) = 0;

    std::uintptr_t dataAddr = static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x0));
    if (dataAddr != 0)
    {
        std::int32_t count = *reinterpret_cast<std::int32_t*>(dataAddr - 4);
        std::uintptr_t blockHeader = dataAddr - 4;

        // Cursor starts one past the last element (+8), then steps back 28 bytes per entry.
        std::uintptr_t cursor = dataAddr + static_cast<std::uintptr_t>(count) * 28u + 8u;
        for (std::int32_t remaining = count; remaining > 0; --remaining)
        {
            std::uint8_t ownsMatrix = *reinterpret_cast<std::uint8_t*>(cursor - 24);
            cursor -= 28;
            if (ownsMatrix != 0)
            {
                std::uint32_t matrixValue = *reinterpret_cast<std::uint32_t*>(cursor);
                RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixValue));
            }
        }

        result = Mem_Free(reinterpret_cast<void*>(blockHeader));
        CSF_AT(int, self, 0x0) = 0;
    }
    return result;
}

HOOK(0x00408850, RwMatrixArray_DestroyAndFree, "auto");
