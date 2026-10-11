#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ParticleManager_AppendEffect, 0x0051CB50, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(float*, __stdcall, ParticleManager_GetAttachmentPosition, 0x0051DBE0, (float* a1, int a2, int a3));
CSF_FUNC(char, __fastcall, ParticleEffect_SetEndpoints, 0x006DCAF0, (void* self, void* edx, float* a2, float* a3));

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_ParticleEndpointTolerance, 0x00840EFC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_00840f00_CreateImpactEffect(
    C_00840f00* self, void* /*edx*/, uint32_t* event, S_0051c1b0* output)
{
    const uint32_t eventType = CSF_AT(uint32_t, event, 0x04);
    if (eventType != 4 && eventType != 5)
        return;

    struct EndpointData
    {
        float position[3];
        int stateValue;
        int attachment;
    } endpoint;

    endpoint.attachment = CSF_AT(int, event, 0x24);
    const int impactType = eventType == 4
        ? CSF_AT(int, event, 0x60)
        : CSF_AT(int, event, 0x34);
    void* handle = reinterpret_cast<uint8_t*>(event) + 0x08;
    float* eventPosition = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(event) + (eventType == 4 ? 0x34 : 0x38));

    endpoint.position[0] = eventPosition[0];
    endpoint.position[1] = eventPosition[1];
    endpoint.position[2] = eventPosition[2];
    output->field_0008 = 0;

    uint32_t effectId;
    switch (impactType)
    {
    case 0:  effectId = self->field_0008; break;
    case 1:  effectId = self->field_000c; break;
    case 2:  effectId = self->field_0010; break;
    case 3:  effectId = self->field_0014; break;
    case 4:  effectId = self->field_0018; break;
    case 5:  effectId = self->field_001c; break;
    case 11: effectId = self->field_0020; break;
    case 12: effectId = self->field_0024; break;
    case 13: effectId = self->field_0028; break;
    case 20:
    case 30: effectId = self->field_002c; break;
    default: return;
    }

    if (effectId == uint32_t(-1))
        return;

    void* context = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, context, 0x1FCC);
    int definitionAddress =
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, definitions, 0)[2])(definitions, nullptr, effectId);
    void* definition = reinterpret_cast<uint8_t*>(definitionAddress) + 4;

    uint32_t effectRecord[4];
    effectRecord[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    effectRecord[2] = uint32_t(-1);
    effectRecord[3] = uint32_t(-1);
    effectRecord[0] = 0;

    uint32_t creationParameters[2];
    creationParameters[0] = uint32_t(-1);
    creationParameters[1] = effectId;

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    endpoint.stateValue = GetStateValue(state, nullptr, 100);
    if (!(CSF_AT(float, definition, 0x60) * g_MaxPercent >
          static_cast<float>(endpoint.stateValue)))
        return;

    int entity = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(handle));
    float attachmentPosition[3];
    ParticleManager_GetAttachmentPosition(
        attachmentPosition, entity, endpoint.attachment);

    float dx = attachmentPosition[0] - endpoint.position[0];
    float dy = attachmentPosition[1] - endpoint.position[1];
    float dz = attachmentPosition[2] - endpoint.position[2];
    if (!(std::sqrt(dz * dz + dy * dy + dx * dx) >
          g_ParticleEndpointTolerance))
        return;

    void* factory = CSF_AT(void*, self, 0x04);
    void* effect =
        ((void*(__fastcall*)(void*, void*, void*, int, uint32_t*))
            CSF_AT(void**, factory, 0)[2])(
                factory, nullptr, definition, 0, creationParameters);
    if (!effect)
        return;

    ParticleEffect_SetEndpoints(
        effect, nullptr, attachmentPosition, endpoint.position);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[10])(effect, nullptr, 0);

    effectRecord[0] = reinterpret_cast<uint32_t>(effect);
    ParticleManager_AppendEffect(output, nullptr, effectRecord, 0);
}

HOOK(0x0051DD30, C_00840f00_CreateImpactEffect, "auto");
