#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d844_SetDirectionalValue, 0x004093F0, (C_0082d844* self, void* , float a2, char a3));
CSF_FUNC(char, __fastcall, CharActionFsm_SetControllerPosition, 0x004096D0, (void* self, void* , float* position));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CharActionFsm_ActivateWithSound(void* self, void* /*edx*/)
{
    void* actor = CSF_AT(void*, self, 0x14);
    void* controllerOwner = CSF_AT(void*, actor, 0x4C);
    C_0082d844* controller =
        reinterpret_cast<C_0082d844* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controllerOwner, 0)[0x3C / 4])(
                controllerOwner, nullptr);

    if ((CSF_AT(uint8_t, self, 0x44) & 0x20) != 0)
    {
        CharActionFsm_SetControllerPosition(
            controller, nullptr,
            reinterpret_cast<float*>(static_cast<uint8_t*>(self) + 0x58));
    }
    C_0082d844_SetDirectionalValue(controller, nullptr, 90.0f, 0);

    alignas(8) uint8_t soundRequest[0x38];
    uint32_t flags = CSF_AT(uint32_t, self, 0x44);
    CSF_AT(uint8_t, soundRequest, 0x35) = 1;
    CSF_AT(uint8_t, soundRequest, 0x36) = 1;
    CSF_AT(int, soundRequest, 0x00) = 2;
    CSF_AT(int, soundRequest, 0x04) = 2;
    actor = CSF_AT(void*, self, 0x14);
    CSF_AT(uint32_t, self, 0x44) = flags | 0x42;
    CSF_AT(int, soundRequest, 0x28) = -1;
    CSF_AT(int, soundRequest, 0x2C) = -1;
    uint8_t* actorIdentity = static_cast<uint8_t*>(actor) + 0x1C;
    CSF_AT(int, soundRequest, 0x18) = 0;
    CSF_AT(int, soundRequest, 0x1C) = 0;
    CSF_AT(int, soundRequest, 0x20) = 0;
    CSF_AT(void**, soundRequest, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, soundRequest, 0x10) = 0;
    CSF_AT(int, soundRequest, 0x14) = 0;
    CSF_AT(uint8_t, soundRequest, 0x34) = 0;
    CSF_AT(int, soundRequest, 0x08) = -1;
    CSF_AT(int, soundRequest, 0x0C) = -1;
    CSF_AT(int, soundRequest, 0x30) = 0;
    CSF_AT(uint32_t, soundRequest, 0x28) = CSF_AT(uint32_t, actorIdentity, 0x04);
    CSF_AT(uint32_t, soundRequest, 0x2C) = CSF_AT(uint32_t, actorIdentity, 0x08);

    int scene = GetGlobalValue_8BC1E0();
    CSF_AT(int, soundRequest, 0x08) = SceneLoader_GetResourceById(
        reinterpret_cast<uint8_t*>(scene) + 0x22A4, nullptr, 31);
    S_00521df0* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    int result = S_00521df0_PlaySoundRequest(
        soundManager, nullptr, soundRequest, -1.0f);

    if (CSF_AT(int, self, 0x6C) >= 0)
    {
        alignas(8) uint8_t recordBytes[0x34];
        C_0082d478& record = *reinterpret_cast<C_0082d478*>(recordBytes);

        record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        actorIdentity = static_cast<uint8_t*>(CSF_AT(void*, self, 0x14)) + 0x1C;
        record.field_000c = static_cast<uint32_t>(-1);
        record.field_0010 = static_cast<uint32_t>(-1);
        record.field_0018 = static_cast<uint32_t>(-1);
        record.field_001c = static_cast<uint32_t>(-1);
        record.vtbl = CSF_VTBL(0x0082D478);
        record.field_0004 = 38;
        record.field_0020 = 0;
        record.field_0024 = 0;
        record.field_002c = 0;
        record.field_0028 = 0;
        record.field_0030 = 0;
        record.field_000c = CSF_AT(uint32_t, actorIdentity, 0x04);
        uint32_t actorId = CSF_AT(uint32_t, actorIdentity, 0x08);
        uint32_t targetId = CSF_AT(uint32_t, self, 0x68);
        uint32_t targetGeneration = CSF_AT(uint32_t, self, 0x6C);
        record.field_0010 = actorId;
        record.field_0018 = targetId;
        record.field_001c = targetGeneration;

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        result = static_cast<int>(
            DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 0));
    }

    void* pendingAction = CSF_AT(void*, self, 0x54);
    if (pendingAction)
        CSF_AT(uint32_t, pendingAction, 0x04) &= ~1u;

    return result;
}

HOOK(0x004D3BF0, CharActionFsm_ActivateWithSound, "auto");
