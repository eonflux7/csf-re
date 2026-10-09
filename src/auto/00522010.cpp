#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_UpdateSpatialSound, 0x00521C10,
         (void* self, void* edx, uint32_t* a2, int a3, float a4));
CSF_FUNC(int, __stdcall, AudioSoundManager_UpdateSound, 0x00521CB0,
         (int a1, int a2));
CSF_FUNC(void, __fastcall, AudioSoundManager_StopSound, 0x005269B0,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, AudioSoundManager_ReleaseSoundHandle, 0x00526CF0,
         (void* self, void* edx, unsigned int* a2));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static void __fastcall S_00522010_UpdateSoundInstances(
    S_00522010* self, void* /*edx*/, int updateArgument)
{
    if (self->field_0079)
        return;

    // Preserve the full stack object passed to ReleaseSoundHandle.
    struct UpdateState
    {
        unsigned int handle;
        float distance;
        uint32_t remaining;
        uint8_t padding[3];
        uint8_t moved;
    } state{};

    state.handle = self->field_0054;
    const uint32_t count = state.handle;
    if (!count)
        return;

    float position[3]{};
    void* listener = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, listener, 0)[0x44 / 4])(listener, nullptr, position);

    float displacement[3] = {
        position[0] - self->field_0068,
        position[1] - self->field_006c,
        position[2] - self->field_0070
    };
    const double distance = RwV3dLength(displacement);
    state.distance = static_cast<float>(distance);

    if (distance > g_Const200_0f)
    {
        self->field_0068 = position[0];
        self->field_006c = position[1];
        state.moved = 1;
        self->field_0070 = position[2];
    }

    uint32_t offset = 0;
    state.remaining = count;
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_004c) + offset;
        if (CSF_AT(uint8_t, entry, 0))
        {
            void* sound = CSF_AT(void*, entry, 0x8);
            if (CSF_AT(uint8_t, sound, 0x3C) &&
                reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sound, 0)[0x3C / 4])(sound, nullptr))
            {
                if (state.moved)
                {
                    AudioSoundManager_UpdateSpatialSound(
                        self, nullptr, reinterpret_cast<uint32_t*>(entry),
                        static_cast<int>(reinterpret_cast<intptr_t>(position)),
                        state.distance);
                }
                AudioSoundManager_UpdateSound(
                    updateArgument, CSF_AT(int, entry, 0x8));
            }
        }
        offset += 12;
        --state.remaining;
    } while (state.remaining);

    uint32_t remaining = state.handle;
    if (!remaining)
        return;

    offset = 0;
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_004c) + offset;
        if (CSF_AT(uint8_t, entry, 0))
        {
            void* sound = CSF_AT(void*, entry, 0x8);
            if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sound, 0)[0x48 / 4])(sound, nullptr))
            {
                sound = CSF_AT(void*, entry, 0x8);
                state.handle = CSF_AT(unsigned int, sound, 0x60);
                if (state.handle)
                {
                    AudioSoundManager_StopSound(sound, nullptr, 0);
                    AudioSoundManager_ReleaseSoundHandle(
                        reinterpret_cast<void*>(self->field_0040),
                        nullptr, &state.handle);
                }
                CSF_AT(uint8_t, entry, 0) = 0;
            }
        }
        offset += 12;
        --remaining;
    } while (remaining);
}

HOOK(0x00522010, S_00522010_UpdateSoundInstances, "auto");
