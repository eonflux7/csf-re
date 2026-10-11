#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SubmitSceneEffect, 0x0048B280, (void* self, void* edx, float* arg0));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_EffectHeightOffset, 0x0083E8A4);

static int __fastcall C_0083e7d8_StartEffect(C_0083e7d8* self, void* /*edx*/)
{
    void* manager = reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
    uint32_t light = ((uint32_t(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, manager, 0)[0x28 / 4])(manager, nullptr, 2, 3);
    self->field_0038 = light;

    void* lightObject = reinterpret_cast<void*>(light);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, lightObject, 0)[0x2C / 4])(lightObject, nullptr, 1);

    // The original color and position temporaries overlap.
    alignas(8) uint8_t lightParameters[0x14] = {};
    lightObject = reinterpret_cast<void*>(self->field_0038);
    CSF_AT(float, lightParameters, 0x00) = 0.5f;
    CSF_AT(float, lightParameters, 0x04) = 0.4f;
    CSF_AT(float, lightParameters, 0x08) = 0.2f;
    CSF_AT(float, lightParameters, 0x0C) = 1.0f;
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, lightObject, 0)[0x34 / 4])(lightObject, nullptr, lightParameters);

    lightObject = reinterpret_cast<void*>(self->field_0038);
    ((void(__fastcall*)(void*, void*, float))
        CSF_AT(void**, lightObject, 0)[0x3C / 4])(lightObject, nullptr, 3000.0f);

    void* position = GetObjectPositionVector(reinterpret_cast<void*>(self->field_000c), nullptr);
    uint32_t positionZ = CSF_AT(uint32_t, position, 0x08);
    position = GetObjectPositionVector(reinterpret_cast<void*>(self->field_000c), nullptr);
    float elevatedY = CSF_AT(float, position, 0x04) + g_EffectHeightOffset;
    position = GetObjectPositionVector(reinterpret_cast<void*>(self->field_000c), nullptr);
    uint32_t positionX = CSF_AT(uint32_t, position, 0x00);

    CSF_AT(float, lightParameters, 0x08) = elevatedY;
    CSF_AT(uint32_t, lightParameters, 0x0C) = positionZ;
    lightObject = reinterpret_cast<void*>(self->field_0038);
    CSF_AT(uint32_t, lightParameters, 0x04) = positionX;
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, lightObject, 0)[0x44 / 4])(
            lightObject, nullptr, lightParameters + 0x04);

    void* owner = reinterpret_cast<void*>(self->field_000c);
    bool hadFlag = (CSF_AT(uint32_t, owner, 0x170) & 0x20) == 32;
    CSF_AT(uint32_t, owner, 0x170) &= 0xFFFFFFDF;

    alignas(8) uint8_t sceneEffect[0x28] = {};
    owner = reinterpret_cast<void*>(self->field_000c);
    CSF_AT(uint32_t, sceneEffect, 0x04) = 0;
    CSF_AT(uint32_t, sceneEffect, 0x08) = 0;
    CSF_AT(uint32_t, sceneEffect, 0x0C) = 0;
    CSF_AT(void*, sceneEffect, 0x00) = owner;
    position = GetObjectPositionVector(owner, nullptr);
    CSF_AT(uint32_t, sceneEffect, 0x04) = CSF_AT(uint32_t, position, 0x00);
    CSF_AT(uint32_t, sceneEffect, 0x08) = CSF_AT(uint32_t, position, 0x04);
    CSF_AT(uint32_t, sceneEffect, 0x0C) = CSF_AT(uint32_t, position, 0x08);

    owner = reinterpret_cast<void*>(self->field_000c);
    CSF_AT(uint32_t, sceneEffect, 0x20) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x150);
    CSF_AT(uint32_t, sceneEffect, 0x24) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x150);
    CSF_AT(int, sceneEffect, 0x14) = -1;
    CSF_AT(uint32_t, sceneEffect, 0x18) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x154);
    CSF_AT(float, sceneEffect, 0x1C) = 1.0f;

    // This address is also classified as a vtable by the supplied packet.
    (void)CSF_VTBL(0x0048B280);
    SubmitSceneEffect(CSF_AT(void*, g_pCurrentScene, 0xA8), nullptr,
                      reinterpret_cast<float*>(sceneEffect));

    owner = reinterpret_cast<void*>(self->field_000c);
    uint32_t flags = CSF_AT(uint32_t, owner, 0x170);
    CSF_AT(uint32_t, owner, 0x170) =
        hadFlag ? flags | 0x20 : flags & 0xFFFFFFDF;

    alignas(8) uint8_t effectRecord[0x60] = {};
    owner = reinterpret_cast<void*>(self->field_000c);
    CSF_AT(void**, effectRecord, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, effectRecord, 0x0C) = -1;
    CSF_AT(int, effectRecord, 0x10) = -1;
    CSF_AT(void**, effectRecord, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, effectRecord, 0x18) = -1;
    CSF_AT(int, effectRecord, 0x1C) = -1;
    CSF_AT(int, effectRecord, 0x04) = 14;
    CSF_AT(int, effectRecord, 0x20) = 0;
    CSF_AT(int, effectRecord, 0x2C) = 0;
    CSF_AT(int, effectRecord, 0x28) = 0;
    CSF_AT(uint8_t, effectRecord, 0x30) = 0;
    CSF_AT(void**, effectRecord, 0x00) = CSF_VTBL(0x008310F8);
    CSF_AT(int, effectRecord, 0x34) = 0;
    CSF_AT(int, effectRecord, 0x38) = 0;
    CSF_AT(int, effectRecord, 0x3C) = 0;
    CSF_AT(int, effectRecord, 0x48) = 0;
    CSF_AT(int, effectRecord, 0x4C) = 0;
    CSF_AT(int, effectRecord, 0x50) = 0;
    CSF_AT(int, effectRecord, 0x54) = 0;
    CSF_AT(int, effectRecord, 0x58) = 0;
    CSF_AT(uint8_t, effectRecord, 0x5C) = 0;
    CSF_AT(int, effectRecord, 0x24) = 4;

    position = GetObjectPositionVector(owner, nullptr);
    CSF_AT(uint32_t, effectRecord, 0x34) = CSF_AT(uint32_t, position, 0x00);
    CSF_AT(uint32_t, effectRecord, 0x38) = CSF_AT(uint32_t, position, 0x04);
    CSF_AT(uint32_t, effectRecord, 0x3C) = CSF_AT(uint32_t, position, 0x08);
    owner = reinterpret_cast<void*>(self->field_000c);
    CSF_AT(void*, effectRecord, 0x40) = owner;
    CSF_AT(float, effectRecord, 0x28) = 15.0f;
    CSF_AT(int, effectRecord, 0x44) = 0;
    CSF_AT(uint32_t, effectRecord, 0x54) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x154);
    CSF_AT(uint32_t, effectRecord, 0x58) =
        CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x150);
    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, effectRecord, 1);

    void* loader = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int resource = SceneLoader_GetResourceById(
        static_cast<uint8_t*>(loader) + 0x22A4, nullptr, 28);

    alignas(8) uint8_t soundRequestBytes[0x40] = {};
    S_00526d40& soundRequest = *reinterpret_cast<S_00526d40*>(soundRequestBytes);
    owner = reinterpret_cast<void*>(self->field_000c);
    CSF_AT(int, soundRequestBytes, 0x18) = 0;
    CSF_AT(int, soundRequestBytes, 0x1C) = 0;
    CSF_AT(int, soundRequestBytes, 0x20) = 0;
    CSF_AT(void**, soundRequestBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, soundRequestBytes, 0x28) = -1;
    CSF_AT(int, soundRequestBytes, 0x2C) = -1;
    soundRequest.field_0000 = 1;
    CSF_AT(int, soundRequestBytes, 0x04) = 2;
    CSF_AT(float, soundRequestBytes, 0x10) = 15.0f;
    CSF_AT(int, soundRequestBytes, 0x14) = 0;
    CSF_AT(uint8_t, soundRequestBytes, 0x34) = 0;
    CSF_AT(uint8_t, soundRequestBytes, 0x35) = 1;
    CSF_AT(uint8_t, soundRequestBytes, 0x36) = 1;
    CSF_AT(int, soundRequestBytes, 0x08) = resource;
    CSF_AT(int, soundRequestBytes, 0x0C) = -1;
    CSF_AT(int, soundRequestBytes, 0x30) = 0;
    position = GetObjectPositionVector(owner, nullptr);
    CSF_AT(uint32_t, soundRequestBytes, 0x18) = CSF_AT(uint32_t, position, 0x00);
    CSF_AT(uint32_t, soundRequestBytes, 0x1C) = CSF_AT(uint32_t, position, 0x04);
    CSF_AT(uint32_t, soundRequestBytes, 0x20) = CSF_AT(uint32_t, position, 0x08);

    auto* soundSystem = reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    int result = S_00521df0_PlaySoundRequest(soundSystem, nullptr, &soundRequest, -1.0f);
    self->field_0028 = 0;
    self->field_003c = 0.0f;
    self->field_0040 = 0.0f;
    self->field_004c = 1;
    self->field_0044 = 15.0f;
    return result;
}

HOOK(0x004E4910, C_0083e7d8_StartEffect, "auto");
