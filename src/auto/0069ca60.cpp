#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, ReadCurrentTickCounter, 0x0069C9B0, (void* self, void* edx));
CSF_GLOBAL(double, g_FrameRateTimeScale, 0x0085E910);

// Returns elapsed milliseconds since the stored start tick. When the flag at +0x20 is set,
// the end tick is the stored value at +0x28; otherwise the current tick counter is read.
static double __fastcall C_0085e92c_GetElapsedMilliseconds(C_0085e92c* self, void* /*edx*/)
{
    if (self->field_0020) {
        uint64_t endTick = *reinterpret_cast<uint64_t*>(&self->field_0028);
        int64_t elapsedTicks = (int64_t)(endTick - *reinterpret_cast<uint64_t*>(&self->field_0008));
        return (double)elapsedTicks * g_FrameRateTimeScale / (double)(int64_t)self->field_0018;
    }

    uint64_t nowTick = ReadCurrentTickCounter(self, nullptr);
    int64_t elapsedTicks = (int64_t)(nowTick - *reinterpret_cast<uint64_t*>(&self->field_0008));
    return (double)elapsedTicks * g_FrameRateTimeScale / (double)(int64_t)self->field_0018;
}

HOOK(0x0069CA60, C_0085e92c_GetElapsedMilliseconds, "auto");
