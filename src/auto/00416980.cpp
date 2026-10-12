#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_InitGlobalRequest, 0x00405A00, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

static void __fastcall C_0082e090_PlayActivitySound(
    C_0082e090* self, void* /*edx*/, uint32_t* eventData)
{
    if (self->field_007c || self->field_00a8 < g_SoundActivityThreshold)
        return;

    alignas(8) uint8_t requestBytes[0x38];
    S_00526d40& request = *reinterpret_cast<S_00526d40*>(requestBytes);
    S_00526d40_InitGlobalRequest(&request, nullptr);

    CSF_AT(uint32_t, &request, 0x18) = eventData[2];
    const uint32_t positionY = eventData[3];
    CSF_AT(uint32_t, &request, 0x20) = eventData[4];
    auto* owner = reinterpret_cast<void*>(self->field_0004);
    CSF_AT(uint32_t, &request, 0x1C) = positionY;
    request.field_0000 = 1;

    int resourceIndex = -1;
    switch (CSF_AT(uint32_t, owner, 0x50))
    {
    case 8:
        resourceIndex = 20;
        break;
    case 9:
        if (self->field_0098 == 2)
            resourceIndex = 26;
        break;
    case 0xB:
        if (self->field_0098 == 1)
            resourceIndex = 23;
        break;
    case 0xE:
    case 0xF:
        resourceIndex = 19;
        break;
    case 0x11:
        if (self->field_0098 == 5)
            resourceIndex = 25;
        break;
    case 0x14:
        if (!eventData[19])
            resourceIndex = 22;
        break;
    case 0x15:
        if (!eventData[19])
            resourceIndex = 21;
        break;
    default:
        break;
    }

    if (resourceIndex != -1)
    {
        auto* scene = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        CSF_AT(int, &request, 0x08) =
            SceneLoader_GetResourceById(scene + 0x22A4, nullptr, resourceIndex);
    }

    if (CSF_AT(int, &request, 0x08) != -1)
    {
        auto* soundManager =
            reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
        const int soundHandle =
            S_00521df0_PlaySoundRequest(soundManager, nullptr, &request, -1.0f);

        if (soundHandle)
        {
            void* sound = reinterpret_cast<void*>(soundHandle);
            ((void (__fastcall*)(void*, void*, uint32_t, uint32_t))
                CSF_AT(void**, sound, 0)[0x28 / 4])(sound, nullptr, 0, 0);
            self->field_00a8 = 0.0f;
        }
    }
}

HOOK(0x00416980, C_0082e090_PlayActivitySound, "auto");
