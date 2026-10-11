#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void, __fastcall, sub_50D700, 0x0050D700, (void* self, void* edx, char a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the audio object reached through GetGlobalValue_8B056C()->[+0x18]->[+0x0C].
static void* GetAudioObject()
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
    uint8_t* holder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, globalValue, 0x18)));
    return reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, holder, 0x0C)));
}

static void __fastcall C_008407a8_SetAudioDuckState(C_008407a8* self, void* /*edx*/, char a2)
{
    if (static_cast<uint8_t>(a2) != self->field_0070)
    {
        sub_50D700(self, nullptr, a2);
        if (a2)
        {
            if (self->field_013c >= g_fZero)
            {
                void* audio = GetAudioObject();
                uint32_t savedBits;
                std::memcpy(&savedBits, &self->field_013c, sizeof(savedBits));
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(CSF_AT(void**, audio, 0)[11])(audio, nullptr, savedBits);
            }
        }
        else
        {
            void* audioForRead = GetAudioObject();
            self->field_013c = reinterpret_cast<float(__fastcall*)(void*, void*)>(CSF_AT(void**, audioForRead, 0)[10])(audioForRead, nullptr);
            void* audioForSet = GetAudioObject();
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(CSF_AT(void**, audioForSet, 0)[11])(audioForSet, nullptr, 0x3f000000u);
        }
    }
}

HOOK(0x511C60, C_008407a8_SetAudioDuckState, "auto");
