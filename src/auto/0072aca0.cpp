#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_TransitionZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);

CSF_FUNC(int, __fastcall, ApplyTransitionFactor, 0x00718C00,
         (void* self, void* edx, int factorBits));

static int __fastcall UpdateSineCosineTransition(
    void* self, void* /*edx*/, float deltaTime)
{
    const int state =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x194 / 4])(self, nullptr);

    if (state != 1)
        return state;

    const double elapsed =
        static_cast<double>(deltaTime) +
        static_cast<double>(CSF_AT(float, self, 0x180));

    // The original compares the unrounded sum before storing it.
    const bool withinDuration =
        elapsed <= static_cast<double>(CSF_AT(float, self, 0x178));

    CSF_AT(float, self, 0x180) = static_cast<float>(elapsed);
    const std::uint8_t increasing = CSF_AT(std::uint8_t, self, 0x17C);

    if (withinDuration)
    {
        const double angle =
            (elapsed / static_cast<double>(CSF_AT(float, self, 0x178))) *
            static_cast<double>(g_TransitionHalfPi);

        // FSIN/FCOS leave finite arguments outside their range unchanged.
        double factor = angle;
        if (!std::isfinite(angle) || std::fabs(angle) < 0x1p63)
            factor = increasing ? std::sin(angle) : std::cos(angle);

        CSF_AT(float, self, 0x184) = static_cast<float>(factor);
    }
    else
    {
        const float factor =
            increasing ? g_TransitionOne : g_TransitionZero;
        void** transitionVtable = CSF_AT(void**, self, 0);

        CSF_AT(float, self, 0x184) = factor;
        CSF_AT(std::uint32_t, self, 0x180) = 0;

        ((void(__fastcall*)(void*, void*, int))
             transitionVtable[0x23C / 4])(self, nullptr, 2);

        void* completionObject =
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x14C));
        if (completionObject)
        {
            ((void(__fastcall*)(void*, void*))
                 CSF_AT(void**, completionObject, 0)[0x19C / 4])(
                     completionObject, nullptr);
        }
    }

    const std::uint32_t childrenBegin =
        CSF_AT(std::uint32_t, self, 0x138);
    std::uint32_t childCount = 0;
    if (childrenBegin)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, self, 0x13C) - childrenBegin;
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteCount) >> 2);
    }

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        const auto children = reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, self, 0x138));
        void* child = reinterpret_cast<void*>(children[index]);
        const std::uint32_t factorBits =
            CSF_AT(std::uint32_t, self, 0x184);

        ((void(__fastcall*)(void*, void*, std::uint32_t))
             CSF_AT(void**, child, 0)[0x1AC / 4])(
                 child, nullptr, factorBits);
    }

    return ApplyTransitionFactor(
        self, nullptr, CSF_AT(int, self, 0x184));
}

HOOK(0x0072ACA0, UpdateSineCosineTransition, "auto");
