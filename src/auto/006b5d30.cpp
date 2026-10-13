#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game debug heap free used by the original (Mem_FreeDebug).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the texture record range owned by the loader (begin = +4, end = +8).
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

// Clears the loader's owned pointer pair (+4 buffer, +8 secondary) and its counter (+12),
// releasing the +4 buffer through the record-range reset first.
static int __cdecl GfxTextureLoader_ClearOwnedBlockPair(int* a1)
{
    int result = CSF_AT(int, a1, 0x4);
    if (result)
    {
        GfxTextureLoader_ResetRecordRange(reinterpret_cast<int*>(CSF_AT(int, a1, 0x4)),
                                          reinterpret_cast<int*>(CSF_AT(int, a1, 0x8)));

        result = CSF_AT(int, a1, 0x4);
        if (result)
        {
            result = Mem_FreeDebug(CSF_AT(void*, a1, 0x4));
        }
    }

    CSF_AT(uint32_t, a1, 0x4) = 0;
    CSF_AT(uint32_t, a1, 0x8) = 0;
    CSF_AT(uint32_t, a1, 0xC) = 0;
    return result;
}

HOOK(0x006B5D30, GfxTextureLoader_ClearOwnedBlockPair, "auto");
