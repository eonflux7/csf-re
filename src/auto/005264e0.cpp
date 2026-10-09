#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Returns true when the global audio object reports an active sound source
// and this instance's state field is not 3. Returns false when the global
// audio object yields no sub-object.
static bool __fastcall C_00840ff0_IsAudioActiveForState(C_00840ff0* self, void* /*edx*/)
{
    void* globalAudio = reinterpret_cast<void*>(static_cast<intptr_t>(GetGlobalValue_70DDE0()));

    // globalAudio->vtbl[0x70 / 4](globalAudio)
    void* audioSource = reinterpret_cast<void*>(
        ((intptr_t(__fastcall*)(void*, void*))CSF_AT(void**, globalAudio, 0)[0x70 / 4])(globalAudio, nullptr));

    if (audioSource == nullptr)
        return false;

    bool stateIsNotThree = (self->field_0004 != 3);

    // audioSource->vtbl[0x3C / 4](audioSource, stateIsNotThree)
    int result = ((int(__fastcall*)(void*, void*, bool))CSF_AT(void**, audioSource, 0)[0x3C / 4])(
        audioSource, nullptr, stateIsNotThree);

    return result != 0;
}

HOOK(0x005264E0, C_00840ff0_IsAudioActiveForState, "auto");
