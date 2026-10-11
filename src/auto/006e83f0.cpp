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

// Engine instance pointer (RenderWare engine object). Its vtable slot 0x108 is the
// allocator the original calls with (byteCount, 0).
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x8B68C0);

// Copies the first dword of every 16-byte keyframe entry of an HAnim hierarchy
// into a freshly allocated dword array. Returns that array.
static int __cdecl GfxHAnim_CopyFirstDwordsToNewArray(int hierarchy)
{
    void* engine = g_pRwEngineInstance;

    // Allocate count*4 bytes via the engine's allocator (vtable slot 0x108 / 4 = 66).
    // The original cleans the two pushed args itself (add esp, 8).
    int entryCount = CSF_AT(int, reinterpret_cast<void*>(hierarchy), 4);
    int result = ((int(__thiscall*)(void*, int, int))CSF_AT(void**, engine, 0)[66])(
        engine, 4 * entryCount, 0);

    int* outArray = reinterpret_cast<int*>(result);
    int index = 0;
    if (CSF_AT(int, reinterpret_cast<void*>(hierarchy), 4) > 0)
    {
        int offset = 0;
        do
        {
            // Source array base and count are re-read every iteration, as in the original.
            uintptr_t base = static_cast<uintptr_t>(CSF_AT(int, reinterpret_cast<void*>(hierarchy), 0x10));
            int value = *reinterpret_cast<int*>(base + static_cast<uintptr_t>(offset));
            outArray[index] = value;
            ++index;
            offset += 16;
        } while (index < CSF_AT(int, reinterpret_cast<void*>(hierarchy), 4));
    }
    return result;
}

HOOK(0x6E83F0, GfxHAnim_CopyFirstDwordsToNewArray, "auto");
