#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t __fastcall Ptank_UpdateGroups(void* self, void* /*edx*/, int updateArgument)
{
    uint32_t result = CSF_AT(uint32_t, self, 0xA4);
    if (!result)
        return result;

    if (CSF_AT(uint32_t, self, 0xF0))
    {
        for (uint32_t groupIndex = 0;
             groupIndex < CSF_AT(uint32_t, self, 0xD0);
             ++groupIndex)
        {
            void* entries = reinterpret_cast<uint8_t*>(
                CSF_AT(void**, self, 0xC8)[groupIndex]) + 0x20;
            uint32_t entryIndex = CSF_AT(uint32_t, self, 0xF0);

            while (entryIndex < static_cast<uint32_t>(
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, entries, 0)[20 / 4])(entries, nullptr)))
            {
                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xC8)[groupIndex]) + 0x20;
                void** table = CSF_AT(void**, entries, 0);
                int entry = ((int (__fastcall*)(void*, void*, uint32_t))
                    table[24 / 4])(entries, nullptr, entryIndex);
                ((void (__fastcall*)(void*, void*, int))
                    table[16 / 4])(entries, nullptr, entry);

                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xC8)[groupIndex]) + 0x20;
                ++entryIndex;
            }
        }
    }

    if (CSF_AT(uint32_t, self, 0xE8))
    {
        for (uint32_t groupIndex = 0;
             groupIndex < CSF_AT(uint32_t, self, 0xB0);
             ++groupIndex)
        {
            void* entries = reinterpret_cast<uint8_t*>(
                CSF_AT(void**, self, 0xA8)[groupIndex]) + 0x20;
            uint32_t entryIndex = CSF_AT(uint32_t, self, 0xE8);

            while (entryIndex < static_cast<uint32_t>(
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, entries, 0)[20 / 4])(entries, nullptr)))
            {
                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xA8)[groupIndex]) + 0x20;
                void** table = CSF_AT(void**, entries, 0);
                int entry = ((int (__fastcall*)(void*, void*, uint32_t))
                    table[24 / 4])(entries, nullptr, entryIndex);
                ((void (__fastcall*)(void*, void*, int))
                    table[16 / 4])(entries, nullptr, entry);

                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xA8)[groupIndex]) + 0x20;
                ++entryIndex;
            }
        }
    }

    if (CSF_AT(uint32_t, self, 0xEC))
    {
        for (uint32_t groupIndex = 0;
             groupIndex < CSF_AT(uint32_t, self, 0xC0);
             ++groupIndex)
        {
            void* entries = reinterpret_cast<uint8_t*>(
                CSF_AT(void**, self, 0xB8)[groupIndex]) + 0x20;
            uint32_t entryIndex = CSF_AT(uint32_t, self, 0xEC);

            while (entryIndex < static_cast<uint32_t>(
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, entries, 0)[20 / 4])(entries, nullptr)))
            {
                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xB8)[groupIndex]) + 0x20;
                void** table = CSF_AT(void**, entries, 0);
                int entry = ((int (__fastcall*)(void*, void*, uint32_t))
                    table[24 / 4])(entries, nullptr, entryIndex);
                ((void (__fastcall*)(void*, void*, int))
                    table[16 / 4])(entries, nullptr, entry);

                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xB8)[groupIndex]) + 0x20;
                ++entryIndex;
            }
        }
    }

    if (CSF_AT(uint32_t, self, 0xF4))
    {
        for (uint32_t groupIndex = 0;
             groupIndex < CSF_AT(uint32_t, self, 0xE0);
             ++groupIndex)
        {
            void* entries = reinterpret_cast<uint8_t*>(
                CSF_AT(void**, self, 0xD8)[groupIndex]) + 0x20;
            uint32_t entryIndex = CSF_AT(uint32_t, self, 0xF4);

            while (entryIndex < static_cast<uint32_t>(
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, entries, 0)[20 / 4])(entries, nullptr)))
            {
                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xD8)[groupIndex]) + 0x20;
                void** table = CSF_AT(void**, entries, 0);
                int entry = ((int (__fastcall*)(void*, void*, uint32_t))
                    table[24 / 4])(entries, nullptr, entryIndex);
                ((void (__fastcall*)(void*, void*, int))
                    table[16 / 4])(entries, nullptr, entry);

                entries = reinterpret_cast<uint8_t*>(
                    CSF_AT(void**, self, 0xD8)[groupIndex]) + 0x20;
                ++entryIndex;
            }
        }
    }

    for (uint32_t groupIndex = 0;
         groupIndex < CSF_AT(uint32_t, self, 0xD0);
         ++groupIndex)
    {
        void* group = CSF_AT(void**, self, 0xC8)[groupIndex];
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, group, 0)[48 / 4])(group, nullptr, updateArgument);
    }

    for (uint32_t groupIndex = 0;
         groupIndex < CSF_AT(uint32_t, self, 0xB0);
         ++groupIndex)
    {
        void* group = CSF_AT(void**, self, 0xA8)[groupIndex];
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, group, 0)[48 / 4])(group, nullptr, updateArgument);
    }

    for (uint32_t groupIndex = 0;
         groupIndex < CSF_AT(uint32_t, self, 0xC0);
         ++groupIndex)
    {
        void* group = CSF_AT(void**, self, 0xB8)[groupIndex];
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, group, 0)[48 / 4])(group, nullptr, updateArgument);
    }

    for (uint32_t groupIndex = 0;
         groupIndex < CSF_AT(uint32_t, self, 0xE0);
         ++groupIndex)
    {
        void* group = CSF_AT(void**, self, 0xD8)[groupIndex];
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, group, 0)[48 / 4])(group, nullptr, updateArgument);
    }

    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    result = CSF_AT(uint32_t, self, 0x100);
    CSF_AT(uint32_t, self, 0x134) = result;
    return result;
}

HOOK(0x006D7930, Ptank_UpdateGroups, "auto");
