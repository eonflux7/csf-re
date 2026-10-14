#include <cstdint>
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static void* __fastcall RecordArray_Grow(void* self, void*, int growth)
{
    auto* record = static_cast<std::uint32_t*>(self);
    auto* block = reinterpret_cast<void*>(CoreMemory_AllocUntagged(
        12u * (record[1] + static_cast<std::uint32_t>(growth))));
    if (record[2])
        std::memcpy(block, reinterpret_cast<void*>(record[0]), 12u * record[2]);
    void* old = reinterpret_cast<void*>(record[0]);
    record[1] += static_cast<std::uint32_t>(growth);
    void* result = block;
    if (old) {
        // The original returns Free's EAX but installs the saved allocation
        // (006D9F0A loads it into EDX; 006D9F17 stores EDX through this).
        result = reinterpret_cast<void*>(CoreMemory_Free(old));
        record[0] = 0;
    }
    record[0] = reinterpret_cast<std::uint32_t>(block);
    return result;
}

HOOK(0x006D9EB0, RecordArray_Grow, "auto");
