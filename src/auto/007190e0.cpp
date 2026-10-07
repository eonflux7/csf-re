#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetColorAnim(
    CGuiControl* self, void* /*edx*/, void* animation)
{
    int result;
    void* previousAnimation = reinterpret_cast<void*>(self->m_pColorAnim);

    if (previousAnimation)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousAnimation, 0)[0])(
                previousAnimation, nullptr, 1);
        self->m_pColorAnim = 0;
    }

    self->m_colorAnimTime = 0.0f;
    self->m_pColorAnim = reinterpret_cast<std::uintptr_t>(animation);

    if (animation)
    {
        std::uint32_t sampledColor[4];
        std::uint32_t color[4];

        reinterpret_cast<void (__fastcall*)(
            void*, void*, std::uint32_t*, std::uint32_t)>(
                CSF_AT(void**, animation, 0)[0x14 / 4])(
                    animation, nullptr, sampledColor, 0);

        color[0] = sampledColor[0];
        color[2] = sampledColor[2];
        color[1] = sampledColor[1];
        void** controlVtable = self->vtbl;
        color[3] = sampledColor[3];

        return reinterpret_cast<int (__fastcall*)(
            CGuiControl*, void*, std::uint32_t*)>(
                controlVtable[0xD0 / 4])(self, nullptr, color);
    }

    return result;
}

HOOK(0x007190E0, CGuiControl_SetColorAnim, "auto");
