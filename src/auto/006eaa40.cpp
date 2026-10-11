#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorCopy, 0x0061F120, (uint32_t* a1, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_RefreshAnimation, 0x006E8BF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_PrepareTransition, 0x006E8D80, (void* self, void* edx, int* a2));
CSF_FUNC(int, __stdcall, GfxHanim_ResetAnimationState, 0x006E8DF0, (uint32_t* a1));
CSF_FUNC(int, __fastcall, C_00861e98_GetAnimationState, 0x006E9030, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_ApplyHierarchy, 0x006E9BF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_RefreshHierarchy, 0x006E9D60, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_00861e98_StopAnimation(
    C_00861e98* self, void* /*edx*/, int animationIndex, int /*unused*/)
{
    if ((CSF_AT(uint8_t, self, 0x58) & 4) != 0 ||
        CSF_AT(uint32_t, self, 0x1C) == 0)
        return;

    auto* state = reinterpret_cast<uint32_t*>(
        C_00861e98_GetAnimationState(self, nullptr, animationIndex));
    uint32_t flags = CSF_AT(uint32_t, state, 0x14) | 0x20;
    CSF_AT(uint32_t, state, 0x14) = flags;

    if (animationIndex == 0)
    {
        CSF_AT(uint32_t, state, 0x14) = flags & 0xFFFFFFFB;
        if (CSF_AT(uint32_t, self, 0x20) != 0)
        {
            auto* animation = CSF_AT(uint32_t*, self, 0x20);
            auto* hierarchy = CSF_AT(int*, self, 0x14);
            RtAnimInterpolatorSetCurrentAnim(
                CSF_AT(uint32_t*, hierarchy, 0x20), animation);
            RpHAnimHierarchyUpdateMatrices(CSF_AT(int*, self, 0x14));

            CSF_AT(uint32_t, state, 0x1C) = 0;
            uint32_t hierarchyFlags = CSF_AT(uint32_t, self, 0x58);
            int currentHierarchy = CSF_AT(int, self, 0x14);
            CSF_AT(uint32_t, self, 0x58) = hierarchyFlags | 0x10;
            C_00861e98_ApplyHierarchy(self, nullptr, currentHierarchy);
            C_00861e98_RefreshHierarchy(self, nullptr);
        }

        GfxHanim_ResetAnimationState(state);
        C_00861e98_RefreshAnimation(self, nullptr);
        return;
    }

    if ((flags & 4) != 0 && (flags & 2) != 0)
    {
        float transitionDuration = CSF_AT(float, state, 0x10);
        CSF_AT(uint32_t, state, 0x14) = flags & 0xFFFFFFF7;
        if (transitionDuration > g_fZero)
        {
            C_00861e98_PrepareTransition(
                self, nullptr, reinterpret_cast<int*>(state));

            auto* sourceHierarchy = CSF_AT(void*, state, 0x00);
            int sourceInterpolator = CSF_AT(int, sourceHierarchy, 0x20);
            auto* transition = CSF_AT(void*, state, 0x04);
            auto* destinationHierarchy = CSF_AT(void*, transition, 0x08);
            auto* destinationInterpolator =
                CSF_AT(uint32_t*, destinationHierarchy, 0x20);
            RtAnimInterpolatorCopy(destinationInterpolator, sourceInterpolator);

            transition = CSF_AT(void*, state, 0x04);
            CSF_AT(uint32_t, transition, 0x14) =
                CSF_AT(uint32_t, state, 0x1C);

            uint32_t transitionFlags = CSF_AT(uint32_t, state, 0x14) | 0x10;
            uint32_t durationBits = CSF_AT(uint32_t, state, 0x10);
            CSF_AT(uint32_t, state, 0x08) = durationBits;
            CSF_AT(uint32_t, state, 0x0C) = 0;
            CSF_AT(uint32_t, state, 0x14) = transitionFlags;
            C_00861e98_RefreshAnimation(self, nullptr);
            return;
        }

        GfxHanim_ResetAnimationState(state);
        CSF_AT(uint32_t, state, 0x14) &= ~4u;
        C_00861e98_RefreshHierarchy(self, nullptr);
    }

    C_00861e98_RefreshAnimation(self, nullptr);
}

HOOK(0x006EAA40, C_00861e98_StopAnimation, "auto");
