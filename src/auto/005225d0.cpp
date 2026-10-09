#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));

static int __fastcall S_00521df0_ReleaseCurrentGroupSounds(
    S_00521df0* self, void* /*edx*/)
{
    uint32_t remaining = CSF_AT(uint32_t, self, 0x18);
    if (remaining)
    {
        uint32_t offset = 0;
        do
        {
            auto* entry = CSF_AT(uint8_t*, self, 0x10) + offset;
            if (CSF_AT(uint32_t, entry, 0) == CSF_AT(uint32_t, self, 0x5C))
            {
                void* sound = CSF_AT(void*, entry, 4);
                if (sound)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

                    auto* reference = reinterpret_cast<unsigned int*>(
                        CSF_AT(uint8_t*, self, 0x10) + offset + 4);
                    sub_526CF0(reinterpret_cast<void*>(self->field_0040),
                              nullptr, reference);
                }
            }
            offset += 12;
        } while (--remaining);
    }

    remaining = CSF_AT(uint32_t, self, 0x28);
    if (remaining)
    {
        uint32_t offset = 0;
        do
        {
            auto* entry = CSF_AT(uint8_t*, self, 0x20) + offset;
            if (CSF_AT(uint32_t, entry, 0) == CSF_AT(uint32_t, self, 0x5C))
            {
                void* sound = CSF_AT(void*, entry, 4);
                if (sound)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

                    auto* reference = reinterpret_cast<unsigned int*>(
                        CSF_AT(uint8_t*, self, 0x20) + offset + 4);
                    sub_526CF0(reinterpret_cast<void*>(self->field_0040),
                              nullptr, reference);
                }
            }
            offset += 24;
        } while (--remaining);
    }

    remaining = self->field_0038;
    if (remaining)
    {
        uint32_t offset = 0;
        do
        {
            auto* entry = reinterpret_cast<uint8_t*>(self->field_0030) + offset;
            if (CSF_AT(uint32_t, entry, 0) == CSF_AT(uint32_t, self, 0x5C))
            {
                void* sound = CSF_AT(void*, entry, 4);
                if (sound)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

                    auto* reference = reinterpret_cast<unsigned int*>(
                        reinterpret_cast<uint8_t*>(self->field_0030) + offset + 4);
                    sub_526CF0(reinterpret_cast<void*>(self->field_0040),
                              nullptr, reference);
                }
            }
            offset += 24;
        } while (--remaining);
    }

    uint32_t group = CSF_AT(uint32_t, self, 0x5C);
    if (group)
    {
        --group;
        CSF_AT(uint32_t, self, 0x5C) = group;
    }
    return static_cast<int>(group);
}

HOOK(0x005225D0, S_00521df0_ReleaseCurrentGroupSounds, "auto");
