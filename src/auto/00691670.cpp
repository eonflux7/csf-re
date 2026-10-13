#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_0069afe0_ProcessSectionTable, 0x0069B460, (S_0069afe0* self, void*));
CSF_FUNC(char, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (S_0069b1c0* self, void* , unsigned int address));

CSF_GLOBAL(S_0069afe0, g_PairBlockPool, 0x008BAF04);

static std::uint32_t* __fastcall CoreMemory_AppendPair(
    void* self, void* /*edx*/, int first, int second)
{
    const std::uint16_t capacity = CSF_AT(std::uint16_t, self, 0x6);
    if (capacity == CSF_AT(std::uint16_t, self, 0x4))
    {
        if (capacity)
        {
            void* replacement = Mem_Alloc(8 * capacity + 32);
            std::memcpy(
                replacement,
                CSF_AT(void*, self, 0x0),
                8 * CSF_AT(std::uint16_t, self, 0x6));

            if (CSF_AT(std::uint16_t, self, 0x6) == 4)
            {
                S_0069b1c0_TryReleaseBlock(
                    reinterpret_cast<S_0069b1c0*>(&g_PairBlockPool),
                    nullptr,
                    CSF_AT(unsigned int, self, 0x0));
            }
            else if (CSF_AT(void*, self, 0x0))
            {
                Mem_Free(CSF_AT(void*, self, 0x0));
                CSF_AT(void*, self, 0x0) = nullptr;
            }

            CSF_AT(std::uint16_t, self, 0x6) += 4;
            CSF_AT(void*, self, 0x0) = replacement;
        }
        else
        {
            CSF_AT(void*, self, 0x0) = reinterpret_cast<void*>(
                S_0069afe0_ProcessSectionTable(&g_PairBlockPool, nullptr));
            CSF_AT(std::uint16_t, self, 0x6) = 4;
        }
    }

    const unsigned int count = CSF_AT(std::uint16_t, self, 0x4);
    auto* entries = CSF_AT(std::uint32_t*, self, 0x0);
    entries[2 * count] = first;
    entries[2 * count + 1] = second;
    ++CSF_AT(std::uint16_t, self, 0x4);
    return entries;
}

HOOK(0x00691670, CoreMemory_AppendPair, "auto");
