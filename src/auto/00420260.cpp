#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct S_0046bea0;
struct S_00521df0;

CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_InitializeRequest, 0x0040FE30, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0, (S_0046bea0* self, void*));
CSF_FUNC(bool, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void*, void* request, float volume));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

static inline uint8_t* AsBytes(int v)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(v)));
}

static void __fastcall C_0082e7c8_UpdateIdleSound(C_0082e7c8* self, void* /*edx*/, float deltaTime)
{
    void* obj = reinterpret_cast<void*>(self->field_0014);
    if (SceneObj_IsInactive(obj, nullptr))
        return;
    obj = reinterpret_cast<void*>(self->field_0014);
    if (SceneObject_PassesFirstCheck(obj, nullptr))
        return;

    self->field_002c = deltaTime + self->field_002c;
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    uint8_t* info = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x4C));

    if (self->field_0024 != 2)
        return;

    float threshold;
    if (CSF_AT(int, owner, 0x1C4) == 1)
        threshold = CSF_AT(float, AsBytes(GetGlobalValue_8BC1E0()), 0x9EC);
    else
        threshold = CSF_AT(float, AsBytes(GetGlobalValue_8BC1E0()), 0x9E8) / CSF_AT(float, info, 0x110);

    if (!(threshold < self->field_002c))
        return;
    if (CSF_AT(uint8_t, info, 0x88) == 0)
        return;

    uint8_t* state = AsBytes(GetGlobalState_50D190());
    void* list = reinterpret_cast<void*>(CSF_AT(uint32_t, state, 0xE8));
    typedef uint8_t* (__fastcall* GetEntryFn)(void*, void*, int);
    uint8_t* entry = ((GetEntryFn)CSF_AT(void**, list, 0)[8 / 4])(list, nullptr, CSF_AT(int, info, 0x40));
    int soundId = CSF_AT(int, entry, 0x2C);

    if (soundId != -1)
    {
        uint8_t* mgr = AsBytes(GetValue8B0554());
        void* soundMgr = reinterpret_cast<void*>(CSF_AT(uint32_t, mgr, 0x1FD0));
        typedef int (__fastcall* LookupFn)(void*, void*, int);
        int idx = ((LookupFn)CSF_AT(void**, soundMgr, 0)[0xC / 4])(soundMgr, nullptr, soundId);

        bool setVariant = false;
        S_0046bea0* script = nullptr;
        if (idx != -1)
        {
            int desc = AudioSoundManager_GetSoundDescriptor(soundMgr, nullptr, idx);
            script = reinterpret_cast<S_0046bea0*>(AsBytes(desc) + 4);
            if (static_cast<unsigned int>(MissionScript_GetEntryCount_46BEA0(script, nullptr)) > 1)
                setVariant = true;
        }

        if (!setVariant)
        {
            self->field_0030 = 0xFFFFFFFFu;
        }
        else
        {
            unsigned int value = static_cast<unsigned int>(
                GlobalStateValue_50DCF0(reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190())), nullptr));
            unsigned int count = static_cast<unsigned int>(MissionScript_GetEntryCount_46BEA0(script, nullptr));
            unsigned int rem = value % count;
            if (rem == self->field_0030)
            {
                if (rem == 0)
                {
                    self->field_0030 = 1;
                    goto play;
                }
                --rem;
            }
            self->field_0030 = rem;
        }
    play:
        uint32_t request[0x38 / 4];
        AudioSoundManager_InitializeRequest(request, nullptr, 4, soundId, 4, 0, static_cast<int>(self->field_0030));
        S_00521df0* player = reinterpret_cast<S_00521df0*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
        S_00521df0_PlaySoundRequest(player, nullptr, request, -1.0f);
    }
    self->field_002c = 0.0f;
}

HOOK(0x00420260, C_0082e7c8_UpdateIdleSound, "auto");
