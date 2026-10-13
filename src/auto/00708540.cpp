#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtAnimAnimationGetNumNodes, 0x0061EFB0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_00863430_SetCurrentAnimation(
    C_00863430* self, void* /*edx*/, int /*unused*/,
    int* animationReference, int parametersAddress)
{
    uint32_t* animation = nullptr;
    if (animationReference)
        animation = reinterpret_cast<uint32_t*>(*animationReference);

    if (!animation)
    {
        animation = reinterpret_cast<uint32_t*>(self->field_0008);
        if (!animation)
            return 0;
    }

    void* parameters = reinterpret_cast<void*>(parametersAddress);
    float time = CSF_AT(float, parameters, 0x00);
    if (time < g_fZero || time > CSF_AT(float, animation, 0x0C))
        time = 0.0f;

    void* hierarchy = reinterpret_cast<void*>(self->field_0004);
    const int animationNodeCount = RtAnimAnimationGetNumNodes(animation);
    if (animationNodeCount != CSF_AT(int, hierarchy, 0x04))
    {
        RtAnimAnimationGetNumNodes(animation);
        return 0;
    }

    RtAnimInterpolatorSetCurrentAnim(
        reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, hierarchy, 0x20)),
        animation);

    RtAnimInterpolatorSetCurrentTime(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x20),
        time);

    const bool looping = CSF_AT(uint8_t, parameters, 0x0C) != 0;
    uint32_t flags = self->field_0014;
    flags = looping ? (flags | 0x10) : (flags & 0xFFFFFFEF);

    hierarchy = reinterpret_cast<void*>(self->field_0004);
    self->field_0014 = flags;
    self->field_0014 = (flags & 0xFFFFFF9F) | 0x20;

    const int nodeCount = CSF_AT(int, hierarchy, 0x04);
    if (self->field_000c)
    {
        for (int i = 0; i < nodeCount; ++i)
        {
            void* node = reinterpret_cast<void*>(
                CSF_AT(uint32_t,
                       reinterpret_cast<void*>(self->field_000c),
                       4 * i));
            if (node)
                CSF_AT(uint8_t, node, 0x20) = 1;
        }
    }

    flags = self->field_0014 | 8;
    self->field_0018 = reinterpret_cast<uint32_t>(animationReference);
    self->field_0014 = flags;

    if (CSF_AT(uint8_t, parameters, 0x1D))
        self->field_0014 = flags | 4;
    else
        self->field_0014 = flags & 0xFFFFFFFB;

    return 1;
}

HOOK(0x00708540, C_00863430_SetCurrentAnimation, "auto");
