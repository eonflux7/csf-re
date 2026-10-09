#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));

static int __fastcall AudioSoundManager_DispatchGlobalValueMessage(
    void* self, void* /*edx*/, const void* message)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x28);
    std::uint32_t index = 0;

    if (static_cast<std::int32_t>(count) >= 4)
    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x20);
        do
        {
            for (unsigned int lane = 0; lane < 4; ++lane)
            {
                if (CSF_AT(std::uint32_t, entry, 0x04) != 0 &&
                    CSF_AT(std::uint32_t, entry, 0x0C) ==
                        CSF_AT(std::uint32_t, message, 0x04) &&
                    CSF_AT(float, entry, 0x14) ==
                        CSF_AT(float, message, 0x0C))
                {
                    goto dispatch;
                }

                ++index;
                entry += 24;
            }
        } while (index < count - 3);
    }

    if (index >= count)
        return -1;

    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x20) + 24 * index;
        for (;;)
        {
            if (CSF_AT(std::uint32_t, entry, 0x04) != 0 &&
                CSF_AT(std::uint32_t, entry, 0x0C) ==
                    CSF_AT(std::uint32_t, message, 0x04) &&
                CSF_AT(float, entry, 0x14) ==
                    CSF_AT(float, message, 0x0C))
            {
                break;
            }

            ++index;
            entry += 24;
            if (index >= count)
                return -1;
        }
    }

dispatch:
    if (index >= count)
        return -1;

    auto* entries = CSF_AT(std::uint8_t*, self, 0x20);
    void* object = CSF_AT(void*, entries + 24 * index, 0x04);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);

    entries = CSF_AT(std::uint8_t*, self, 0x20);
    auto* objectSlot = reinterpret_cast<unsigned int*>(entries + 24 * index + 4);
    void* owner = CSF_AT(void*, self, 0x40);
    sub_526CF0(owner, nullptr, objectSlot);

    return static_cast<int>(index);
}

HOOK(0x00522330, AudioSoundManager_DispatchGlobalValueMessage, "auto");
