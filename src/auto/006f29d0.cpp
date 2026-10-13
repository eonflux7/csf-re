#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __fastcall, CCamera_GetReferencedTransitionMetric, 0x006D2AA0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, ParticleEmitters_LookupParticleSlot, 0x006E10F0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, ParticleEmitters_LookupSlotAddress, 0x006E1110, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, CommitOverlayBuffer, 0x006EF050, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ParticleRender_CopyTwoDwordsAtOffsets30And34, 0x00705210, (void* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, ParticleRender_SetValueClampedToUnit, 0x007052C0, (void* self, void* , int rawValueBits));
CSF_FUNC(char, __fastcall, S_00705300_RenderBlendedPasses, 0x00705600, (S_00705300* self, void*));
CSF_FUNC(int, __fastcall, C_008656d0_SetField002C, 0x00713F90, (C_008656d0* self, void* , int value));

static int __fastcall C_00862598_RenderParticlesAndOverlay(
    C_00862598* self, void* /*edx*/)
{
    alignas(8) uint8_t displacementBytes[0x10];
    float* displacement = reinterpret_cast<float*>(displacementBytes);

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    const float dz = self->field_0038 - CSF_AT(float, globalState, 0x12C);
    const float dy = self->field_0034 - CSF_AT(float, globalState, 0x128);
    float positionX;
    std::memcpy(&positionX, &self->field_0030, sizeof(positionX));
    displacement[0] = positionX - CSF_AT(float, globalState, 0x124);
    displacement[1] = dy;
    displacement[2] = dz;

    const double distance = RwV3dLength(displacement);
    float scale = static_cast<float>(
        CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x7C) / distance);

    globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    const double transitionMetric = CCamera_GetReferencedTransitionMetric(
        CSF_AT(void*, globalState, 0xF0), nullptr);
    const double threshold = transitionMetric *
        CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x80);

    if (scale < threshold)
        scale = static_cast<float>(threshold / scale);
    else
        scale = 1.0f;

    int scaleBits;
    std::memcpy(&scaleBits, &scale, sizeof(scaleBits));

    const int renderSlotIndex = static_cast<int>(self->field_0028);
    void* emitters = reinterpret_cast<void*>(GetValue_8BB738());
    void* renderSlot = reinterpret_cast<void*>(
        ParticleEmitters_LookupSlotAddress(emitters, nullptr, renderSlotIndex));
    void* renderer = CSF_AT(void*, renderSlot, 0x1C);

    ParticleRender_CopyTwoDwordsAtOffsets30And34(
        renderer, nullptr, &self->field_003c, &self->field_0040);
    ParticleRender_SetValueClampedToUnit(
        renderer, nullptr, static_cast<int>(self->field_0044));

    CSF_AT(uint32_t, renderer, 0x04) = self->field_0030;
    CSF_AT(float, renderer, 0x08) = self->field_0034;
    CSF_AT(float, renderer, 0x0C) = self->field_0038;
    C_008656d0_SetField002C(
        reinterpret_cast<C_008656d0*>(renderer), nullptr, scaleBits);
    S_00705300_RenderBlendedPasses(
        reinterpret_cast<S_00705300*>(renderer), nullptr);

    const int particleSlotIndex = static_cast<int>(self->field_0024);
    emitters = reinterpret_cast<void*>(GetValue_8BB738());
    void* particleSlot = reinterpret_cast<void*>(
        ParticleEmitters_LookupParticleSlot(emitters, nullptr, particleSlotIndex));
    void* overlay = CSF_AT(void*, particleSlot, 0x1C);

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, overlay, 0)[0x20 / 4])(
            overlay, nullptr, &self->field_0030);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, overlay, 0)[0x2C / 4])(
            overlay, nullptr, scaleBits);

    return CommitOverlayBuffer(overlay, nullptr);
}

HOOK(0x006F29D0, C_00862598_RenderParticlesAndOverlay, "auto");
