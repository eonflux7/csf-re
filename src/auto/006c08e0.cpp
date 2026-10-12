#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static S_00402350* __fastcall GfxFrames_GrowTransformArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    // Retain the vtable binding listed by the packet; initialization is delegated.
    (void)CSF_VTBL(0x00402350);

    const uint32_t capacity =
        CSF_AT(uint32_t, self, 0x04) +
        static_cast<uint32_t>(additionalCapacity);

    auto* replacement = static_cast<S_00402350*>(
        Mem_Alloc(static_cast<uint32_t>(60 * capacity)));

    if (replacement)
    {
        uint32_t remaining = capacity - 1;
        if (static_cast<int32_t>(remaining) >= 0)
        {
            ++remaining;
            auto* transformBytes = reinterpret_cast<uint8_t*>(replacement);
            do
            {
                S00402350_InitTransform(
                    reinterpret_cast<S_00402350*>(transformBytes), nullptr);
                transformBytes += 60;
                --remaining;
            }
            while (remaining);
        }
    }

    const uint32_t count = CSF_AT(uint32_t, self, 0x08);
    if (count != 0)
    {
        std::memcpy(
            replacement,
            CSF_AT(void*, self, 0x00),
            static_cast<uint32_t>(60 * count));
    }

    const uint32_t updatedCapacity =
        CSF_AT(uint32_t, self, 0x04) +
        static_cast<uint32_t>(additionalCapacity);
    void* oldBlock = CSF_AT(void*, self, 0x00);
    CSF_AT(uint32_t, self, 0x04) = updatedCapacity;

    if (oldBlock)
    {
        const int freeResult = Mem_Free(oldBlock);
        CSF_AT(void*, self, 0x00) = nullptr;
        CSF_AT(void*, self, 0x00) = replacement;
        return reinterpret_cast<S_00402350*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(freeResult)));
    }

    CSF_AT(void*, self, 0x00) = replacement;
    return replacement;
}

HOOK(0x006C08E0, GfxFrames_GrowTransformArray, "auto");
