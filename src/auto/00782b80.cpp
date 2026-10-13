#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Each C_0082d410 entry in the range is 0x15C bytes; within an entry the
// four 0x50-byte blocks sit at offsets 0x1C, 0x6C, 0xBC and 0x10C.
static constexpr std::size_t kRangeEntryStride = 0x15C;
static constexpr std::size_t kBlockBytes = 0x50;

static void __cdecl CopyStateBlocksToRange(C_0082d410* a1, C_0082d410* a2, int a3)
{
    if (a1 == a2)
        return;

    const std::uint32_t sourceAddress = static_cast<std::uint32_t>(a3);
    const auto* source = reinterpret_cast<const std::uint8_t*>(sourceAddress);
    const auto* rangeEnd = reinterpret_cast<std::uint8_t*>(a2);
    auto* entry = reinterpret_cast<std::uint8_t*>(a1);

    do {
        C_0082d410* entryObj = reinterpret_cast<C_0082d410*>(entry);
        AssignStringSubstring(entryObj, nullptr, reinterpret_cast<const void*>(sourceAddress), 0u, 0xFFFFFFFFu);

        std::memcpy(entry + 0x1C,  source + 0x1C,  kBlockBytes);
        std::memcpy(entry + 0x6C,  source + 0x6C,  kBlockBytes);
        std::memcpy(entry + 0xBC,  source + 0xBC,  kBlockBytes);
        std::memcpy(entry + 0x10C, source + 0x10C, kBlockBytes);

        entry += kRangeEntryStride;
    } while (entry != rangeEnd);
}

HOOK(0x00782B80, CopyStateBlocksToRange, "auto");
