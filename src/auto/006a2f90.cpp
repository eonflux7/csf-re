#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, CoreMemDebug_Allocate, 0x00690DC0, (size_t size, int file, int line));
CSF_FUNC(int, __cdecl, MemDebug_SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __fastcall, RecordArray_Grow, 0x006D9EB0, (void* self, void* edx, int amount));
CSF_GLOBAL(uint8_t*, g_TrackedBlockTable, 0x008A6BAC);
CSF_GLOBAL(uint32_t, g_TrackedBlockCapacity, 0x008A6BB0);
CSF_GLOBAL(uint32_t, g_TrackedBlockCount, 0x008A6BB4);
CSF_GLOBAL(int, g_TrackedBlockGrowth, 0x008A6BB8);

// zalloc(opaque, items, size): opaque is NOT a this pointer. The original
// ignores it; both inflate setup paths publish this cdecl callback at stream+0x20.
static void* __cdecl CoreZlibLog_AllocateReusableBlock(
    void* /*opaque*/, uint32_t items, uint32_t size)
{
    const uint32_t bytes = items * size;
    for (uint32_t index = 0; index < g_TrackedBlockCount; ++index) {
        uint8_t* entry = g_TrackedBlockTable + 12 * index;
        if (CSF_AT(uint8_t, entry, 8) && CSF_AT(uint32_t, entry, 4) == bytes) {
            void* block = CSF_AT(void*, entry, 0);
            CSF_AT(uint8_t, entry, 8) = 0;
            if (block) return block;
            break;
        }
    }
    MemDebug_SetAllocationMode(3);
    void* block = CoreMemDebug_Allocate(bytes, 0, 0);
    MemDebug_SetAllocationMode(0);
    uint32_t index = g_TrackedBlockCount;
    if (index >= g_TrackedBlockCapacity) {
        RecordArray_Grow(&g_TrackedBlockTable, nullptr, g_TrackedBlockGrowth);
        index = g_TrackedBlockCount;
    }
    g_TrackedBlockCount = index + 1;
    uint8_t* entry = g_TrackedBlockTable + 12 * index;
    CSF_AT(void*, entry, 0) = block;
    CSF_AT(uint32_t, entry, 4) = bytes;
    // Only the low byte of the original stack temporary is defined.
    CSF_AT(uint8_t, entry, 8) = 0;
    return block;
}

HOOK(0x006A2F90, CoreZlibLog_AllocateReusableBlock, "auto");
