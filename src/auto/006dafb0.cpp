#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CopyDwordArrayWithContext_552EF0, 0x00552EF0, (int destination, int count, int source, int context));
CSF_FUNC(std::uint32_t, __fastcall, AppendDwordCopies_555B70, 0x00555B70, (void* self, void* edx, std::uint32_t* position, std::uint32_t count, std::int32_t* value));
CSF_FUNC(void, __fastcall, S_006d2390_ClearLights, 0x006D2390, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ReserveFirstCollection, 0x006DAD80, (void* self, void* edx, unsigned int capacity));
CSF_FUNC(char, __fastcall, InitializeEntryCollection, 0x006DAE70, (void* self, void* edx, unsigned int a4, int a3, int _18, int a5, int a6));

// Retain the packet's address annotation without introducing a vtable write.
[[maybe_unused]] static const auto kClearLightsVtableAnnotation =
    CSF_VTBL(0x006D2390);

static char __fastcall S_006d2390_ResolveParam(
    S_006d2390* self, void* /*edx*/, int count, int provider, int mode)
{
    S_006d2390_ClearLights(self, nullptr);
    self->field_0038 = static_cast<std::uint32_t>(provider);

    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    InitializeEntryCollection(bytes + 0x4, nullptr, count, 0, -1, 0, 0);

    void* indexCollection = bytes + 0x14;
    ReserveFirstCollection(indexCollection, nullptr, count);
    ReserveFirstCollection(bytes + 0x24, nullptr, count);

    self->field_003c = static_cast<std::uint8_t>(mode);

    std::int32_t index = 0;
    while (index < count)
    {
        const std::uint32_t entryOffset =
            16 * static_cast<std::uint32_t>(index);
        auto* entry = reinterpret_cast<void*>(self->field_0008 + entryOffset);
        auto* providerObject = reinterpret_cast<void*>(self->field_0038);

        const int value =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, providerObject, 0)[1])(providerObject, nullptr);
        CSF_AT(int, entry, 0xC) = value;

        auto* currentEntry =
            reinterpret_cast<void*>(self->field_0008 + entryOffset);
        const std::int32_t savedIndex = index;
        CSF_AT(std::int32_t, currentEntry, 0x4) = savedIndex;

        const std::uint32_t begin = self->field_0018;
        std::uint32_t size = 0;
        if (begin)
        {
            size = static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x1C) - begin) >> 2;
        }

        if (begin &&
            size < static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, self, 0x20) - begin) >> 2))
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x1C);
            CopyDwordArrayWithContext_552EF0(
                static_cast<int>(end), 1,
                reinterpret_cast<int>(&index),
                reinterpret_cast<int>(indexCollection));
            CSF_AT(std::uint32_t, self, 0x1C) = end + 4;
        }
        else
        {
            AppendDwordCopies_555B70(
                indexCollection, nullptr,
                reinterpret_cast<std::uint32_t*>(
                    CSF_AT(std::uint32_t, self, 0x1C)),
                1u, &index);
        }

        auto* indices = reinterpret_cast<void*>(self->field_0018);
        CSF_AT(std::int32_t, indices, 4 * savedIndex) = savedIndex;
        index = savedIndex + 1;
    }

    self->field_0034 = 0;
    self->field_0000 = 1;
    return 1;
}

HOOK(0x006DAFB0, S_006d2390_ResolveParam, "auto");
