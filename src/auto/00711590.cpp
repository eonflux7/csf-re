#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(std::uint64_t, __cdecl, Audio_AmplitudeToClampedMillibels, 0x007113E0, (float amplitude, int minimumMillibels, int maximumMillibels));

static int __fastcall C_00863cd4_SetVolumeFromAmplitude(
    C_00863cd4* self, void* /*edx*/, float amplitude)
{
    if (amplitude > g_TransitionOne) {
        amplitude = g_TransitionOne;
    } else if (amplitude < g_fZero) {
        amplitude = g_fZero;
    }

    void* buffer = reinterpret_cast<void*>(self->field_0004);
    void** bufferVtable = CSF_AT(void**, buffer, 0);

    int millibels = static_cast<int>(
        Audio_AmplitudeToClampedMillibels(amplitude, -10000, 0));

    // Keep the owner in ECX while passing both COM arguments on the stack.
    // The callee pops eight bytes, just as with its stdcall interface.
    return reinterpret_cast<int(__fastcall*)(C_00863cd4*, void*, void*, int)>(
        bufferVtable[0x3C / 4])(self, nullptr, buffer, millibels);
}

HOOK(0x00711590, C_00863cd4_SetVolumeFromAmplitude, "auto");
