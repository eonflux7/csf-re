#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_InitializeEmitterPlacement, 0x006F35C0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __cdecl, GfxPrtStdParticles_CheckVisibility, 0x006F3D10, (float* a1, float* a2));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, GfxPrtStdParticles_PreparePlacement, 0x006F4420, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_RefreshPlacement, 0x006F4590, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxParticle_RenderEmitterBatches_704370, 0x00704370, (int a1, int));

static int __fastcall C_00862748_UpdateParticleVisibility(
    C_00862748* self, void* /*edx*/)
{
    void* visibilityManager = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0xF0);
    const int objectFlag =
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0018), 0x41) != 0;

    alignas(8) uint8_t placementBytes[0x28];
    alignas(8) uint8_t emitterPlacementBytes[0x28];
    alignas(8) uint8_t boundsBytes[0x10];
    alignas(8) uint8_t visibilityBytes[0x8];
    float& visibilityScale = CSF_AT(float, visibilityBytes, 0);

    bool visible;
    if (self->field_0028)
    {
        GfxPrtStdParticles_PreparePlacement(self, nullptr);
        GfxPrtStdParticles_RefreshPlacement(self, nullptr);

        uint32_t* position = self->vGetGlobal8BB420();
        CSF_AT(uint32_t, boundsBytes, 0) = position[0];
        const float radius =
            CSF_AT(float, reinterpret_cast<void*>(self->field_0024), 0x28);
        CSF_AT(uint32_t, boundsBytes, 4) = position[1];
        CSF_AT(uint32_t, boundsBytes, 8) = position[2];
        CSF_AT(float, boundsBytes, 0xC) = radius;

        visible = reinterpret_cast<bool(__fastcall*)(
            void*, void*, void*, uint32_t*, int)>(
                CSF_AT(void**, visibilityManager, 0)[0x6C / 4])(
                    visibilityManager, nullptr, boundsBytes,
                    &self->field_0080, objectFlag);
    }
    else
    {
        visible = reinterpret_cast<bool(__fastcall*)(
            void*, void*, void*, uint32_t*, int)>(
                CSF_AT(void**, visibilityManager, 0)[0x74 / 4])(
                    visibilityManager, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x44,
                    &self->field_0080, objectFlag);
    }

    if (visible && (self->field_0014 & 0x20) != 0)
    {
        visibilityScale = 1.0f;
        float* position = reinterpret_cast<float*>(self->vGetGlobal8BB420());
        visible = GfxPrtStdParticles_CheckVisibility(position, &visibilityScale);

        if ((self->field_0014 & 8) == 0)
        {
            void* placement =
                GfxPtank_GetPlacementState(self, nullptr, placementBytes);
            void* scaleState = CSF_AT(void*, placement, 0x1C);
            if (CSF_AT(float, scaleState, 4) < visibilityScale)
            {
                RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024));
                GfxParticle_RenderEmitterBatches_704370(
                    static_cast<int>(self->field_0024), 0);
            }
        }

        int emitter =
            RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024));
        GfxPrtStdParticles_InitializeEmitterPlacement(
            emitterPlacementBytes, nullptr, emitter);
        std::memcpy(placementBytes, emitterPlacementBytes, 0x28);
        void* scaleState = CSF_AT(void*, placementBytes, 0x1C);
        CSF_AT(float, scaleState, 4) = visibilityScale;
    }

    return !self->vCheckGlobalObjectFlag() && visible;
}

HOOK(0x006F4DF0, C_00862748_UpdateParticleVisibility, "auto");
