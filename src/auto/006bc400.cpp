#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, GameSleepMilliseconds, 0x0069E450, (uint32_t dwMilliseconds));

static int __fastcall GfxEnvironment_ReserveBufferRange(
    void* self, void* /*edx*/, uint32_t size)
{
    auto* bytes = static_cast<uint8_t*>(self);

    while (CSF_AT(uint32_t, self, 0xC0) >= 0xF ||
           CSF_AT(uint32_t, self, 0xB8) -
                   CSF_AT(uint32_t, self, 0xBC) < size)
    {
        GameSleepMilliseconds(0xAu);
    }

    void* lock = CSF_AT(void*, self, 0x178);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, lock, 0)[1])(lock, nullptr, 0);

    int freeSlot = -1;
    for (uint32_t index = 0; index < 0xF; ++index)
    {
        auto* entry = bytes + 196 + 12 * index;
        if (!CSF_AT(uint32_t, entry, 0) ||
            !CSF_AT(uint8_t, entry, 8))
        {
            freeSlot = static_cast<int>(index);
            break;
        }
    }

    uint32_t candidate = CSF_AT(uint32_t, self, 0xB4);
    uint32_t nextSlot = 0;

    for (;;)
    {
        bool overlaps = false;
        for (uint32_t index = 0; index < 0xF; ++index)
        {
            auto* entry = bytes + 196 + 12 * index;
            uint32_t start = CSF_AT(uint32_t, entry, 0);
            if (start && CSF_AT(uint8_t, entry, 8) == 1)
            {
                uint32_t end = start + CSF_AT(uint32_t, entry, 4);
                if (candidate + size >= start && candidate <= end)
                {
                    overlaps = true;
                    break;
                }
            }
        }

        if (!overlaps)
            break;

        uint32_t index = nextSlot;
        while (index < 0xF)
        {
            auto* entry = bytes + 196 + 12 * index;
            if (CSF_AT(uint8_t, entry, 8))
                break;
            ++index;
        }

        if (index >= 0xF)
        {
            lock = CSF_AT(void*, self, 0x178);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, lock, 0)[2])(lock, nullptr);
            return -1;
        }

        auto* entry = bytes + 196 + 12 * index;
        candidate = CSF_AT(uint32_t, entry, 0) +
                    CSF_AT(uint32_t, entry, 4);
        nextSlot = index + 1;
    }

    auto* entry = bytes + 196 + 12 * freeSlot;
    CSF_AT(uint32_t, entry, 0) = candidate;
    CSF_AT(uint32_t, entry, 4) = size;
    CSF_AT(uint8_t, entry, 8) = 0;

    uint32_t count = CSF_AT(uint32_t, self, 0xC0);
    CSF_AT(uint32_t, self, 0xBC) += size;
    lock = CSF_AT(void*, self, 0x178);
    CSF_AT(uint32_t, self, 0xC0) = count + 1;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, lock, 0)[2])(lock, nullptr);
    return freeSlot;
}

HOOK(0x006BC400, GfxEnvironment_ReserveBufferRange, "auto");
