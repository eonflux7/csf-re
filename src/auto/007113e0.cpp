#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_AudioMinimumAmplitude, 0x00846158);
CSF_GLOBAL(float, g_AudioMillibelScale, 0x00863CD0);

static std::uint64_t __cdecl Audio_AmplitudeToClampedMillibels(
    float amplitude, int minimumMillibels, int maximumMillibels)
{
    const float safeAmplitude =
        amplitude < g_AudioMinimumAmplitude ? g_AudioMinimumAmplitude : amplitude;

    // fldlg2 / fyl2x: log10(2) * log2(x) == log10(x)
    double millibels =
        0.3010299956639812 * std::log2(static_cast<double>(safeAmplitude)) * g_AudioMillibelScale;

    const float upperBound = static_cast<float>(maximumMillibels);
    if (millibels > upperBound)
        millibels = upperBound;

    if (static_cast<double>(minimumMillibels) > millibels)
        millibels = static_cast<float>(minimumMillibels);

    return static_cast<std::uint64_t>(static_cast<std::int64_t>(millibels));
}

HOOK(0x007113E0, Audio_AmplitudeToClampedMillibels, "auto");
