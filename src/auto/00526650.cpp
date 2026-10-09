#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Returns (int)(position * (a2 / duration)) where duration is the voice's length
// (vtbl slot 56) and position is the voice's current offset (vtbl slot 52, read as
// a signed int with a +2^32 bias when negative). Returns 0 when the duration is not positive.
static int __fastcall CSoundInstance_ComputeScaledVoicePosition(CSoundInstance* self, void* /*edx*/, float a2)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    float duration = ((float(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[14])(voice, nullptr);

    // Negated test: NaN also takes the zero-result path, as in the original.
    if (!(duration > g_fZero))
        return 0;

    void* voiceAgain = reinterpret_cast<void*>(self->m_voice);
    int rawPosition = ((int(__fastcall*)(void*, void*))CSF_AT(void**, voiceAgain, 0)[13])(voiceAgain, nullptr);

    float position = static_cast<float>(rawPosition);
    if (rawPosition < 0)
        position += g_UnsignedIntegerBias;

    float scaled = position * (a2 / duration);
    return static_cast<int>(static_cast<int64_t>(scaled));
}

HOOK(0x00526650, CSoundInstance_ComputeScaledVoicePosition, "auto");
