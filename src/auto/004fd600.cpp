#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, IntVector_FillUninitialized, 0x0046BFB0, (std::uint32_t* destination, std::uint32_t count, const std::uint32_t* source, void* context));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall C_0083f448_RebuildProviderIndices(
    C_0083f448* self, void* /*edx*/)
{
    auto* indices = reinterpret_cast<std::uint8_t*>(self) + 0xC0;

    if (self->field_00c4)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_00c4));

    self->field_00c4 = 0;
    CSF_AT(std::uint32_t, self, 0xC8) = 0;
    CSF_AT(std::uint32_t, self, 0xCC) = 0;

    std::uint32_t objectIndex = 0;
    while (objectIndex < self->field_001c)
    {
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void**>(self->field_0014)[objectIndex], nullptr);

        const std::uint32_t providerCount = self->field_000c;
        std::uint32_t providerIndex = 0;
        if (providerCount)
        {
            auto* providers = reinterpret_cast<std::uint32_t*>(self->field_0004);
            while (providerIndex < providerCount &&
                   providers[providerIndex] != reinterpret_cast<std::uintptr_t>(provider))
            {
                ++providerIndex;
            }
        }
        if (providerIndex == providerCount)
            providerIndex = static_cast<std::uint32_t>(-1);

        const std::uint32_t begin = self->field_00c4;
        const std::uint32_t size = begin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      CSF_AT(std::uint32_t, self, 0xC8) - begin) >> 2)
            : 0;

        if (begin &&
            size < static_cast<std::uint32_t>(
                       static_cast<std::int32_t>(
                           CSF_AT(std::uint32_t, self, 0xCC) - begin) >> 2))
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0xC8);
            IntVector_FillUninitialized(
                reinterpret_cast<std::uint32_t*>(end), 1, &providerIndex, indices);
            CSF_AT(std::uint32_t, self, 0xC8) = end + 4;
        }
        else
        {
            IntVector_InsertN(
                indices, nullptr,
                reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 0xC8)),
                1u, &providerIndex);
        }

        ++objectIndex;
    }

    return objectIndex;
}

HOOK(0x004FD600, C_0083f448_RebuildProviderIndices, "auto");
