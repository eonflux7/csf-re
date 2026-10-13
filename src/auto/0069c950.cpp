#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_SecondsToMinutesFactor, 0x0085E928);

// Splits the value returned by vSlot10 (milliseconds) into seconds, minutes and hours
// fields modulo 60, and returns the hours divided by 60.
static int __fastcall C_0085e92c_UpdateElapsedTimeFields(C_0085e92c* self, void* /*edx*/)
{
    double totalSeconds = self->vSlot10() * g_SceneLoadMillisecondsToSeconds;
    float totalMinutes = g_SecondsToMinutesFactor * static_cast<float>(totalSeconds);
    float totalHours = totalMinutes * g_SecondsToMinutesFactor;

    int wholeSeconds = static_cast<int>(static_cast<int64_t>(totalSeconds));
    self->field_0030 = static_cast<uint32_t>(wholeSeconds % 60);

    int wholeMinutes = static_cast<int>(static_cast<int64_t>(totalMinutes));
    self->field_0034 = static_cast<uint32_t>(wholeMinutes % 60);

    int wholeHours = static_cast<int>(static_cast<int64_t>(totalHours));
    self->field_0038 = static_cast<uint32_t>(wholeHours % 60);

    return wholeHours / 60;
}

HOOK(0x0069C950, C_0085e92c_UpdateElapsedTimeFields, "auto");
