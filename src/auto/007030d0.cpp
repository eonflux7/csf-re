#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicDestroy, 0x0058F060, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static int __fastcall S_007030d0_SelectCachedAtomic(
    S_007030d0* self, void* /*edx*/, int bucketIndex, unsigned int requestedSize)
{
    const uint32_t bucketOffset = 16 * static_cast<uint32_t>(bucketIndex);
    auto* bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
    int selectedAtomic = 0;

    if (CSF_AT(uint32_t, bucket, 0x08) == 0)
        return 0;

    int selectedIndex = -1;
    uint32_t smallestSize = 0x7FFFFFFF;
    uint32_t index = 0;
    uint32_t entryOffset = 0;

    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, bucket, 0x00))
                    + entryOffset;
        const uint32_t size = CSF_AT(uint32_t, entry, 0x04);

        if (size < requestedSize ||
            static_cast<double>(requestedSize) * g_ControlHoldThreshold
                <= static_cast<double>(size))
        {
            CSF_AT(uint8_t, entry, 0x08) =
                static_cast<uint8_t>(CSF_AT(uint8_t, entry, 0x08) - 1);
        }
        else
        {
            if (size < smallestSize)
            {
                selectedIndex = static_cast<int>(index);
                smallestSize = size;
            }
            CSF_AT(uint8_t, entry, 0x08) =
                static_cast<uint8_t>(CSF_AT(uint8_t, entry, 0x08) + 1);
        }

        entryOffset += 12;
        bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
        ++index;
    }
    while (index < CSF_AT(uint32_t, bucket, 0x08));

    if (selectedIndex != -1)
    {
        bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
        const uint32_t selectedOffset = 12 * static_cast<uint32_t>(selectedIndex);
        auto* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, bucket, 0x00));
        selectedAtomic = CSF_AT(int, entries + selectedOffset, 0x00);

        const uint32_t remainingCount = CSF_AT(uint32_t, bucket, 0x08) - 1;
        CSF_AT(uint32_t, bucket, 0x08) = remainingCount;
        if (static_cast<uint32_t>(selectedIndex) != remainingCount)
        {
            entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, bucket, 0x00));
            _memcpy(entries + selectedOffset, entries + selectedOffset + 12,
                    12 * (remainingCount - static_cast<uint32_t>(selectedIndex)));
        }
    }

    bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
    int32_t cleanupIndex =
        static_cast<int32_t>(CSF_AT(uint32_t, bucket, 0x08) - 1);
    uint32_t cleanupOffset = 12 * static_cast<uint32_t>(cleanupIndex);

    while (cleanupIndex >= 0)
    {
        bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
        auto* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, bucket, 0x00))
                    + cleanupOffset;

        if (CSF_AT(uint8_t, entry, 0x08) == 0)
        {
            auto* atomic = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, entry, 0x00));
            const int frame = CSF_AT(int, atomic, 0x04);
            if (frame != 0)
            {
                RpAtomicSetFrame(static_cast<int>(CSF_AT(uint32_t, entry, 0x00)), 0);
                RwFrameDestroy(frame);
            }

            RpAtomicDestroy(
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, entry, 0x00)));
            CSF_AT(uint32_t, entry, 0x00) = 0;

            bucket = reinterpret_cast<uint8_t*>(self->field_0000) + bucketOffset;
            const uint32_t remainingCount = CSF_AT(uint32_t, bucket, 0x08) - 1;
            CSF_AT(uint32_t, bucket, 0x08) = remainingCount;

            if (static_cast<uint32_t>(cleanupIndex) != remainingCount)
            {
                auto* entries =
                    reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, bucket, 0x00));
                _memcpy(entries + cleanupOffset, entries + cleanupOffset + 12,
                        12 * (remainingCount - static_cast<uint32_t>(cleanupIndex)));
            }
        }

        --cleanupIndex;
        cleanupOffset -= 12;
    }

    return selectedAtomic;
}

HOOK(0x007030D0, S_007030d0_SelectCachedAtomic, "auto");
