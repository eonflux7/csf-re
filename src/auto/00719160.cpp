#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiControl_CopyColorAnimation(
    CGuiControl* self, void* /*edx*/, CGuiControl* source)
{
    void* previousAnimation = reinterpret_cast<void*>(self->m_pColorAnim);
    if (previousAnimation)
    {
        ((void (__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, previousAnimation, 0)[0])(
                previousAnimation, nullptr, 1);
        self->m_pColorAnim = 0;
    }

    if (!source)
    {
        self->m_colorAnimTime = 0.0f;
        self->m_pColorAnim = 0;
        return;
    }

    self->m_colorAnimTime = source->m_colorAnimTime;
    const uint32_t animationAddress = source->m_pColorAnim;
    self->m_pColorAnim = animationAddress;

    if (animationAddress)
    {
        void* animation = reinterpret_cast<void*>(animationAddress);
        uint32_t sampledColor[4];
        uint32_t color[4];

        ((void (__fastcall*)(void*, void*, uint32_t*, float))
            CSF_AT(void**, animation, 0)[0x14 / 4])(
                animation, nullptr, sampledColor, self->m_colorAnimTime);

        color[2] = sampledColor[2];
        color[0] = sampledColor[0];
        color[1] = sampledColor[1];
        void** destinationVtable = self->vtbl;
        color[3] = sampledColor[3];

        ((void (__fastcall*)(void*, void*, uint32_t*))
            destinationVtable[0xD0 / 4])(self, nullptr, color);
    }
    else
    {
        void** sourceVtable = source->vtbl;
        void** destinationVtable = self->vtbl;
        uint32_t* color =
            ((uint32_t* (__fastcall*)(void*, void*))
                sourceVtable[0xD4 / 4])(source, nullptr);

        ((void (__fastcall*)(void*, void*, uint32_t*))
            destinationVtable[0xD0 / 4])(self, nullptr, color);
    }
}

HOOK(0x00719160, CGuiControl_CopyColorAnimation, "auto");
