#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static void __fastcall Hud_TickFadeEntries(void* self, void* /*edx*/, float deltaSeconds)
{
    int entryIndex = CSF_AT(int, self, 0x234);
    if (entryIndex == 0)
        return;

    entryIndex = static_cast<int>(static_cast<std::uint32_t>(entryIndex) - 1);
    if (entryIndex < 0)
        return;

    std::uint32_t entryOffset = 16u * static_cast<std::uint32_t>(entryIndex);
    do
    {
        std::uint8_t* entries = CSF_AT(std::uint8_t*, self, 0x22C);
        CSF_AT(float, entries, entryOffset + 8) =
            deltaSeconds + CSF_AT(float, entries, entryOffset + 8);

        entries = CSF_AT(std::uint8_t*, self, 0x22C);
        if (!(CSF_AT(float, entries, entryOffset + 8) >=
              CSF_AT(float, entries, entryOffset + 4)))
        {
            const auto globalValue =
                reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
            entries = CSF_AT(std::uint8_t*, self, 0x22C);

            const double fadeStart =
                static_cast<double>(CSF_AT(float, entries, entryOffset + 4)) *
                CSF_AT(float, globalValue, 6356);
            const float elapsed = CSF_AT(float, entries, entryOffset + 8);

            if (elapsed >= fadeStart)
            {
                entries = CSF_AT(std::uint8_t*, self, 0x22C);
                CSF_AT(float, entries, entryOffset + 12) =
                    static_cast<float>(
                        (g_TransitionOne -
                         (elapsed - fadeStart) /
                             (CSF_AT(float, entries, entryOffset + 4) - fadeStart)) *
                        g_ColorChannelScale);
            }
        }
        else
        {
            const int newEntryCount = static_cast<int>(
                static_cast<std::uint32_t>(CSF_AT(int, self, 0x234)) - 1);
            CSF_AT(int, self, 0x234) = newEntryCount;

            if (entryIndex != newEntryCount)
            {
                entries = CSF_AT(std::uint8_t*, self, 0x22C);
                _memcpy(entries + entryOffset,
                        entries + entryOffset + 16,
                        16u * (static_cast<std::uint32_t>(newEntryCount) -
                               static_cast<std::uint32_t>(entryIndex)));
            }
        }

        --entryIndex;
        entryOffset -= 16;
    } while (entryIndex >= 0);
}

HOOK(0x0042F2D0, Hud_TickFadeEntries, "auto");
