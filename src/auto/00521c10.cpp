#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SoundEmitter_EvaluateAudibility, 0x00526AA0, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(void, __fastcall, SoundSource_SetActiveInstance, 0x005269B0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, SoundRegistry_ReleaseInstance, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(uint32_t*, __fastcall, SoundRegistry_FindInstance, 0x00528340, (void* self, void* edx, uint32_t* a2));

// Reconciles the sound instance attached to an emitter's source with its audibility.
// Stack args: emitter (a2), position (a3), radius (a4). ret 12.
static uint32_t* __fastcall SoundManager_UpdateEmitterSoundInstance(void* self, void* /*edx*/, void* emitter, float* position, float radius)
{
    const int audibleFlag = SoundEmitter_EvaluateAudibility(CSF_AT(void*, emitter, 8), nullptr, position, radius);
    const uint8_t audible = static_cast<uint8_t>(audibleFlag);
    // Pass-through paths return the full eax value of the audibility call.
    uint32_t* flagResult = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(audibleFlag)));

    void* source = CSF_AT(void*, emitter, 8);
    void* activeInstance = CSF_AT(void*, source, 0x60);

    if (activeInstance != nullptr) {
        if (audible != 0) {
            return flagResult;
        }
        // Stop the active instance (vtable slot 0x2c).
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, activeInstance, 0)[11])(activeInstance, nullptr);
        SoundSource_SetActiveInstance(CSF_AT(void*, emitter, 8), nullptr, 0);
        return SoundRegistry_ReleaseInstance(CSF_AT(void*, self, 0x40), nullptr, reinterpret_cast<unsigned int*>(&activeInstance));
    }

    if (audible == 0) {
        return flagResult;
    }

    uint32_t* found = SoundRegistry_FindInstance(CSF_AT(void*, self, 0x40), nullptr,
                                                 reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(source) + 4));
    activeInstance = found;
    if (found == nullptr) {
        return found;
    }

    // Vtable slot 0x20: is the found instance playable?
    const uint8_t foundPlayable = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, found, 0)[8])(found, nullptr);
    if (foundPlayable != 0) {
        SoundSource_SetActiveInstance(CSF_AT(void*, emitter, 8), nullptr, static_cast<int>(reinterpret_cast<intptr_t>(activeInstance)));
        return found;
    }

    return SoundRegistry_ReleaseInstance(CSF_AT(void*, self, 0x40), nullptr, reinterpret_cast<unsigned int*>(&activeInstance));
}

HOOK(0x00521C10, SoundManager_UpdateEmitterSoundInstance, "auto");
