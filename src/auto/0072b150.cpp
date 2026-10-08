#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_HandleMouseMovement, 0x00719400,
         (CGuiControl* self, void* edx, void* input));

static char __fastcall CGuiPanel_ProcessInput(
    CGuiPanel* self, void* /*edx*/, void* input)
{
    const char enabled =
        reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr);
    if (!enabled)
        return enabled;

    if (self->m_pPointerQuad)
    {
        const uint32_t x = CSF_AT(uint32_t, input, 0x14);
        const uint32_t y = CSF_AT(uint32_t, input, 0x18);
        uint32_t position[2];
        position[0] = x;
        void* pointerQuad = reinterpret_cast<void*>(self->m_pPointerQuad);
        position[1] = y;

        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, pointerQuad, 0)[0x28 / 4])(
                pointerQuad, nullptr, position);
    }

    const uint32_t childrenBegin = self->m_childrenBegin;
    uint32_t childCount = 0;
    if (childrenBegin)
    {
        childCount = static_cast<uint32_t>(
            static_cast<int32_t>(self->m_childrenEnd - childrenBegin) >> 2);
    }

    for (uint32_t index = 0; index < childCount; ++index)
    {
        void* child =
            reinterpret_cast<void**>(self->m_childrenBegin)[index];
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, child, 0)[0x30 / 4])(child, nullptr, input);
    }

    return CGuiControl_HandleMouseMovement(self, nullptr, input);
}

HOOK(0x0072B150, CGuiPanel_ProcessInput, "auto");
