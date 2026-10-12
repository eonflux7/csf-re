#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static std::uint8_t* __fastcall C_008611d4_RemoveParticle(
    C_008611d4* self, void* /*edx*/, void* indexArgument)
{
    const std::uint32_t index =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(indexArgument));

    const std::uint32_t activeCount = CSF_AT(std::uint32_t, self, 0x110);
    if (activeCount != 0)
    {
        void* particle = CSF_AT(void**, self, 0xF8)[index];
        void** activeParticles = CSF_AT(void**, self, 0x108);
        std::uint32_t activeIndex = 0;

        while (activeIndex < activeCount)
        {
            if (activeParticles[activeIndex] == particle)
            {
                if (activeIndex != 0xFFFFFFFFu)
                {
                    const std::uint32_t lastActiveIndex =
                        CSF_AT(std::uint32_t, self, 0x110) - 1;
                    CSF_AT(std::uint32_t, self, 0x110) = lastActiveIndex;

                    if (activeIndex != lastActiveIndex)
                    {
                        void** destination =
                            CSF_AT(void**, self, 0x108) + activeIndex;
                        _memcpy(destination, destination + 1,
                                4 * (lastActiveIndex - activeIndex));
                    }
                }
                break;
            }
            ++activeIndex;
        }
    }

    void* particle = CSF_AT(void**, self, 0xF8)[index];
    if (particle != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, particle, 0)[1])(particle, nullptr, 1);
        CSF_AT(void**, self, 0xF8)[index] = nullptr;
    }

    const std::uint32_t lastIndex = CSF_AT(std::uint32_t, self, 0x100) - 1;
    CSF_AT(std::uint32_t, self, 0x100) = lastIndex;

    if (index != lastIndex)
    {
        void** destination = CSF_AT(void**, self, 0xF8) + index;
        return static_cast<std::uint8_t*>(
            _memcpy(destination, destination + 1, 4 * (lastIndex - index)));
    }

    return reinterpret_cast<std::uint8_t*>(lastIndex);
}

HOOK(0x006D7860, C_008611d4_RemoveParticle, "auto");
