#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, AiTaskStates_IsMovementFinished, 0x0041E400, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiTaskStates_SetTurnRate, 0x0041E420, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, AiTaskStates_AdvanceMovement, 0x0041E7A0, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, AiTaskStates_SetMovementHeading, 0x0041E7E0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, AiTaskStates_SetMovementSpeed, 0x0041E820, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, AiTaskStates_UpdatePathSpeed, 0x0041EE60, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, AiTaskStates_FindPositionRegion, 0x00420B40, (void* self, void* edx, void* a2));
CSF_FUNC(unsigned int, __fastcall, AiTaskStates_RefreshPosition, 0x00421120, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiTaskStates_ShouldRefreshSpeed, 0x004218D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiTaskStates_UpdateNavigationProgress, 0x00421980, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_UpdateNavigationPosition, 0x00422680, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_AdjustPositionForRegion, 0x00448DD0, (void* self, void* edx, int* a2, int a3));
CSF_FUNC(double, __fastcall, AiTaskStates_ResolveMovementSpeed, 0x0048E510, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_NavigationDegreesToRadians, 0x0082E898);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Address tags supplied for the two direct-call sites; no vtable is written here.
[[maybe_unused]] static const auto kPositionRefreshVtable = CSF_VTBL(0x00421120);
[[maybe_unused]] static const auto kMovementSpeedVtable = CSF_VTBL(0x0041E820);

static char __fastcall S_00421120_UpdateNavigation(
    S_00421120* self, void* /*edx*/, float deltaTime)
{
    auto* actor = reinterpret_cast<S_00421750*>(self->field_0008);
    if (static_cast<uint8_t>(AiTaskStates_IsMovementFinished(
            reinterpret_cast<uint8_t*>(actor) + 0x29C, nullptr)) != 0)
        return 0;

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    float previousPosition[4] = {
        actor->field_0218,
        actor->field_021c,
        actor->field_0220,
        0.0f
    };

    uint32_t deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    auto* nextPosition = reinterpret_cast<float*>(
        AiTaskStates_AdvanceMovement(
            reinterpret_cast<uint8_t*>(actor) + 0x29C,
            nullptr,
            reinterpret_cast<void*>(deltaTimeBits)));

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    actor->field_0218 = nextPosition[0];
    actor->field_021c = nextPosition[1];
    actor->field_0220 = nextPosition[2];

    AiTaskStates_RefreshPosition(self, nullptr);

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    if (CSF_AT(int, actor, 0x1C) == 1)
    {
        const int region = AiTaskStates_FindPositionRegion(
            actor, nullptr, &actor->field_0218);
        if (region != 0 &&
            (CSF_AT(uint32_t, reinterpret_cast<void*>(region), 0x4) & 2) != 0)
        {
            actor = reinterpret_cast<S_00421750*>(self->field_0008);
            AiTaskStates_AdjustPositionForRegion(
                CSF_AT(void*, g_pCurrentScene, 0xC4),
                nullptr,
                reinterpret_cast<int*>(&actor->field_0218),
                region);
        }
    }

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    if (static_cast<uint8_t>(AiTaskStates_IsMovementFinished(
            reinterpret_cast<uint8_t*>(actor) + 0x29C, nullptr)) != 0)
        return 0;

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    float* position = &actor->field_0218;
    if (static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
            position, nullptr, previousPosition)) != 0)
    {
        const float displacementZ = position[2] - previousPosition[2];
        const float displacementY = position[1] - previousPosition[1];
        previousPosition[0] = position[0] - previousPosition[0];
        previousPosition[1] = displacementY;
        previousPosition[2] = displacementZ;

        const float heading = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(
                reinterpret_cast<S_00526d40*>(previousPosition), nullptr));
        reinterpret_cast<S_00421750*>(self->field_0008)->field_023c = heading;

        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        const float turnRate = static_cast<float>(
            ComputeScalarForResolvedObject(
                reinterpret_cast<void*>(actor->field_0014), nullptr) *
            g_NavigationDegreesToRadians);

        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        AiTaskStates_SetTurnRate(
            reinterpret_cast<uint8_t*>(actor) + 0x244, nullptr, turnRate);

        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        AiTaskStates_SetMovementHeading(
            reinterpret_cast<uint8_t*>(actor) + 0x244,
            nullptr,
            actor->field_0240 + actor->field_023c);

        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        AiTaskStates_SetMovementSpeed(
            reinterpret_cast<uint8_t*>(actor) + 0x244,
            nullptr,
            actor->field_0200);
    }

    if (CSF_AT(uint8_t, self, 0x14) == 0 &&
        AiTaskStates_ShouldRefreshSpeed(
            reinterpret_cast<S_00421750*>(self->field_0008), nullptr) != 0)
    {
        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        const int speedSelector = static_cast<int>(actor->field_0238);
        previousPosition[3] = actor->field_0200;
        const float speed = static_cast<float>(
            AiTaskStates_ResolveMovementSpeed(
                reinterpret_cast<void*>(actor->field_0014),
                nullptr,
                speedSelector));

        reinterpret_cast<S_00421750*>(self->field_0008)->field_0200 = speed;
        actor = reinterpret_cast<S_00421750*>(self->field_0008);
        if (previousPosition[3] != actor->field_0200)
        {
            actor = reinterpret_cast<S_00421750*>(self->field_0008);
            AiTaskStates_UpdatePathSpeed(
                reinterpret_cast<uint8_t*>(actor) + 0x29C,
                nullptr,
                actor->field_0200);
        }
    }

    AiTaskStates_UpdateNavigationProgress(
        reinterpret_cast<S_00421750*>(self->field_0008), nullptr, deltaTime);

    actor = reinterpret_cast<S_00421750*>(self->field_0008);
    AiTaskStates_UpdateNavigationPosition(
        actor, nullptr, reinterpret_cast<uint32_t*>(&actor->field_0218));
    return 1;
}

HOOK(0x00422FA0, S_00421120_UpdateNavigation, "auto");
