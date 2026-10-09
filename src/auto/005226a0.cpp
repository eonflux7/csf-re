#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static unsigned char __fastcall AudioSoundManager_FinishLoadContext(
    void* self, void* /*edx*/)
{
    const unsigned char finished = CSF_AT(unsigned char, self, 0x79);
    if (finished)
        return finished;

    std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x18);
    CSF_AT(unsigned char, self, 0x79) = 1;

    if (remaining)
    {
        std::uint32_t offset = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x10);
            void* object = CSF_AT(void*, entries + offset, 0x4);
            if (object &&
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr, 1);
            }
            offset += 12;
            --remaining;
        } while (remaining);
    }

    remaining = CSF_AT(std::uint32_t, self, 0x28);
    if (remaining)
    {
        std::uint32_t offset = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x20);
            void* object = CSF_AT(void*, entries + offset, 0x4);
            if (object &&
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr, 1);
            }
            offset += 24;
            --remaining;
        } while (remaining);
    }

    remaining = CSF_AT(std::uint32_t, self, 0x38);
    if (remaining)
    {
        std::uint32_t offset = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x30);
            void* object = CSF_AT(void*, entries + offset, 0x4);
            if (object &&
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr, 1);
            }
            offset += 24;
            --remaining;
        } while (remaining);
    }

    remaining = CSF_AT(std::uint32_t, self, 0x54);
    if (remaining)
    {
        std::uint32_t offset = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x4C);
            void* object = CSF_AT(void*, entries + offset, 0x8);
            if (object &&
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr, 1);
            }
            offset += 12;
            --remaining;
        } while (remaining);
    }

    return 0;
}

HOOK(0x005226A0, AudioSoundManager_FinishLoadContext, "auto");
