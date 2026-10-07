#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// This entry consumes ST(0), not a normal C++ stack argument.
CSF_FUNC(std::int64_t, __cdecl, X87TruncateToInt64, 0x007AF86C, ());

static void __fastcall CGuiControl_SetPosAnim(
    CGuiControl* self, void* /*edx*/, std::uint32_t positionAnimation)
{
    void* oldAnimation = reinterpret_cast<void*>(self->m_pPosAnim);
    if (oldAnimation)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, oldAnimation, 0)[0])(
                oldAnimation, nullptr, 1);
        self->m_pPosAnim = 0;
    }

    self->m_posAnimTime = 0.0f;
    self->m_pPosAnim = positionAnimation;

    if (positionAnimation)
    {
        float initialPosition[2];
        void* animation = reinterpret_cast<void*>(positionAnimation);

        reinterpret_cast<void(__fastcall*)(
            void*, void*, float*, std::uint32_t)>(
                CSF_AT(void**, animation, 0)[0x14 / 4])(
                    animation, nullptr, initialPosition, 0);

        // Express the x87 helper's truncation without an invalid ABI call.
        (void)X87TruncateToInt64;
        std::uint32_t position[2];
        position[0] = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(initialPosition[0]));
        position[1] = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(initialPosition[1]));

        reinterpret_cast<void(__fastcall*)(
            CGuiControl*, void*, std::uint32_t*)>(
                self->vtbl[0x98 / 4])(self, nullptr, position);
    }
}

HOOK(0x00719220, CGuiControl_SetPosAnim, "auto");
