#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Raw aligned-buffer allocator used by the scene loader (cdecl, 4 stack args).
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));

// Layout used by this routine (no declared struct; offsets from the original):
//   +0x04 : geometry buffer, 36 bytes per entry (a2 entries)
//   +0x08 : entry count (a2)
//   +0x10 : secondary buffer, 2 bytes per entry (a3 entries)
//   +0x14 : secondary entry count (a3)
// The original returns a 32-bit int (xor eax, eax + setne al), so the return type is int.
static int __fastcall SceneLoader_ConfigureGeometry(void* self, void* /*edx*/, int entryCount, int secondaryCount)
{
    CSF_AT(int, self, 0x08) = entryCount;
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(
        AllocateAlignedBuffer(static_cast<std::size_t>(36 * entryCount), 64, 0, 0));

    if (secondaryCount > 0)
    {
        CSF_AT(int, self, 0x14) = secondaryCount;
        CSF_AT(uint32_t, self, 0x10) = static_cast<uint32_t>(
            AllocateAlignedBuffer(static_cast<std::size_t>(2 * secondaryCount), 64, 0, 0));
    }

    return CSF_AT(uint32_t, self, 0x04) != 0 ? 1 : 0;
}

HOOK(0x006C35C0, SceneLoader_ConfigureGeometry, "auto");
