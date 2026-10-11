#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, FxParticleManager_GrowRemovedReferences, 0x0051FCD0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static int __fastcall FxParticleManager_RemoveObjectReference(
    void* self, void* /*edx*/, void* reference)
{
    int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(reference));
    if (CSF_AT(int, reference, 0x08) < 0)
        return result;

    int index = CSF_AT(int, self, 0x64) - 1;
    if (index < 0)
        return result;

    int offset = 36 * index;
    do
    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x5C) + offset;
        if (CSF_AT(std::uint32_t, entry, 0x08) ==
            CSF_AT(std::uint32_t, reference, 0x04))
        {
            void* object = CSF_AT(void*, entry, 0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr, 0);

            void** vtable = CSF_AT(void**, object, 0);
            int value = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                vtable[0x48 / 4])(object, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                vtable[0x38 / 4])(object, nullptr, value);

            entry = CSF_AT(std::uint8_t*, self, 0x5C) + offset;
            std::uint32_t firstReference = CSF_AT(std::uint32_t, entry, 0x1C);
            std::uint32_t secondReference = CSF_AT(std::uint32_t, entry, 0x20);

            if (CSF_AT(std::uint32_t, self, 0x54) >=
                CSF_AT(std::uint32_t, self, 0x50))
            {
                FxParticleManager_GrowRemovedReferences(
                    static_cast<std::uint8_t*>(self) + 0x4C,
                    nullptr, CSF_AT(int, self, 0x58));
            }

            std::uint32_t removedCount = CSF_AT(std::uint32_t, self, 0x54);
            auto* removedEntry =
                CSF_AT(std::uint8_t*, self, 0x4C) + 16 * removedCount;
            CSF_AT(std::uint32_t, self, 0x54) = removedCount + 1;
            CSF_AT(void*, removedEntry, 0x00) = object;
            CSF_AT(std::uint32_t, removedEntry, 0x04) = 0;
            CSF_AT(std::uint32_t, removedEntry, 0x08) = firstReference;
            CSF_AT(std::uint32_t, removedEntry, 0x0C) = secondReference;

            int remainingCount = CSF_AT(int, self, 0x64) - 1;
            CSF_AT(int, self, 0x64) = remainingCount;
            if (index != remainingCount)
            {
                auto* destination =
                    CSF_AT(std::uint8_t*, self, 0x5C) + offset;
                _memcpy(destination, destination + 36,
                        static_cast<std::size_t>(36 * (remainingCount - index)));
            }
        }

        --index;
        offset -= 36;
        result = offset;
    } while (index >= 0);

    return result;
}

HOOK(0x005201D0, FxParticleManager_RemoveObjectReference, "auto");
