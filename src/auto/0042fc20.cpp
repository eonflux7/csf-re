#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Owner layout (no class declared, so accessed by offset):
//   +0x00 void* elements   (buffer of 12-byte records)
//   +0x04 int   capacity   (record count; grows by extraElements)
//   +0x08 int   usedCount  (records copied from the old buffer)

static const void* __fastcall Hud_GrowElementArray(void* self, void* /*edx*/, int extraElements)
{
    // New capacity = current capacity + extra, computed before allocating.
    const std::int32_t newCapacity = CSF_AT(std::int32_t, self, 0x04) + extraElements;

    void* newElements = nullptr;
    void* allocated = Mem_Alloc(static_cast<std::size_t>(newCapacity) * 12u);
    if (allocated != nullptr)
    {
        newElements = allocated;
        if (newCapacity > 0)
        {
            // Zero every 12-byte record of the new buffer (three dwords each).
            auto* words = static_cast<std::uint32_t*>(newElements);
            const std::int32_t wordCount = newCapacity * 3;
            for (std::int32_t i = 0; i < wordCount; ++i)
            {
                words[i] = 0;
            }
        }
    }

    // Preserve the used records from the old buffer into the new one.
    const std::int32_t usedCount = CSF_AT(std::int32_t, self, 0x08);
    if (usedCount != 0)
    {
        const auto* source = static_cast<const std::uint32_t*>(CSF_AT(const void*, self, 0x00));
        auto* destination = static_cast<std::uint32_t*>(newElements);
        const std::uint32_t dwordCount = static_cast<std::uint32_t>(usedCount) * 3u;
        for (std::uint32_t i = 0; i < dwordCount; ++i)
        {
            destination[i] = source[i];
        }
    }

    void* oldElements = CSF_AT(void*, self, 0x00);
    const void* result = oldElements;
    const bool hadOldElements = (oldElements != nullptr);

    CSF_AT(std::int32_t, self, 0x04) += extraElements;

    if (hadOldElements)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::intptr_t>(Mem_Free(oldElements)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newElements;
    return result;
}

HOOK(0x0042FC20, Hud_GrowElementArray, "auto");
