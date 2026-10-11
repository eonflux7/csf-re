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

CSF_FUNC(int, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, void* requester, int a, int b));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, CheckGlobalObjectFlag, 0x00521C00, (void* self, void* edx));
CSF_FUNC(double, __fastcall, ConvertValueToScalar, 0x00523140, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* edx, void* request, float volume));
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

static int __fastcall Autonomous_PlaySoundAndStartAction(void* self, void* /*edx*/, int arg0, int a3)
{
    alignas(8) uint8_t reqBytes[0x38];
    uint8_t* owner = CSF_AT(uint8_t*, self, 8);

    CSF_AT(uint32_t, owner, 0x18) |= 1u;
    C_0082f9f8_TryAcquire(CSF_AT(void*, owner, 0x14), nullptr, self, 0, 1);

    void* soundMgr = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554())), 0x1fd0);
    int soundId = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, soundMgr, 0)[3])(soundMgr, nullptr, a3);
    if (soundId != -1)
        AudioSoundManager_GetSoundDescriptor(soundMgr, nullptr, soundId);

    CSF_AT(int, self, 0x14) = 0;

    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
    if (CheckGlobalObjectFlag(globalObj, nullptr))
    {
        CSF_AT(int, reqBytes, 0x18) = 0;            // ebp-0x20
        CSF_AT(int, reqBytes, 0x1c) = 0;            // ebp-0x1c
        CSF_AT(int, reqBytes, 0x20) = 0;            // ebp-0x18
        CSF_AT(void*, reqBytes, 0x24) = CSF_VTBL(0x0082D448);
        CSF_AT(int, reqBytes, 0x28) = -1;
        CSF_AT(int, reqBytes, 0x2c) = -1;
        CSF_AT(int, reqBytes, 0x00) = 1;
        CSF_AT(int, reqBytes, 0x04) = 1;
        CSF_AT(int, reqBytes, 0x10) = 0;
        CSF_AT(int, reqBytes, 0x14) = 0;
        CSF_AT(uint8_t, reqBytes, 0x34) = 0;
        CSF_AT(uint8_t, reqBytes, 0x35) = 1;
        CSF_AT(uint8_t, reqBytes, 0x36) = 1;
        CSF_AT(int, reqBytes, 0x08) = -1;
        CSF_AT(int, reqBytes, 0x0c) = -1;
        CSF_AT(int, reqBytes, 0x30) = 0;

        if (a3 != -1)
        {
            uint8_t* target = CSF_AT(uint8_t*, CSF_AT(uint8_t*, self, 8), 0x14);
            CSF_AT(int, reqBytes, 0x28) = CSF_AT(int, target, 0x20);
            CSF_AT(int, reqBytes, 0x2c) = CSF_AT(int, target, 0x24);
            CSF_AT(int, reqBytes, 0x08) = a3;
            CSF_AT(int, reqBytes, 0x10) = 0;
            CSF_AT(int, reqBytes, 0x04) = 4;
            CSF_AT(int, reqBytes, 0x00) = 5;
            S_00521df0* mgr = reinterpret_cast<S_00521df0*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
            CSF_AT(int, self, 0x14) = S_00521df0_PlaySoundRequest(mgr, nullptr, reqBytes, -2.0f);
        }
    }

    if (CSF_AT(int, self, 0x14) != 0)
    {
        void* target = CSF_AT(void*, CSF_AT(uint8_t*, self, 8), 0x14);
        return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, target, 0)[0x90 / 4])(
            target, nullptr, arg0, 0, -1, 0);
    }

    void* scalarObj = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
    float scalar = static_cast<float>(ConvertValueToScalar(scalarObj, nullptr, a3));
    int scalarBits;
    std::memcpy(&scalarBits, &scalar, sizeof(scalarBits));

    void* target = CSF_AT(void*, CSF_AT(uint8_t*, self, 8), 0x14);
    return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, target, 0)[0x90 / 4])(
        target, nullptr, arg0, scalarBits, -1, 0);
}

HOOK(0x004D8950, Autonomous_PlaySoundAndStartAction, "auto");
