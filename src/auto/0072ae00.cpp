#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_PrepareListRendering, 0x00719A20,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiPanel_Draw(CGuiPanel* self, void* /*edx*/)
{
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    void* background = reinterpret_cast<void*>(self->m_pBackgroundQuad);
    if (background)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, background, 0)[8 / 4])(background, nullptr);
    }

    CGuiListBoxBase_PrepareListRendering(self, nullptr);

    const std::uint32_t childrenBegin = self->m_childrenBegin;
    std::int32_t childCount = 0;
    if (childrenBegin != 0)
    {
        childCount = static_cast<std::int32_t>(
            self->m_childrenEnd - childrenBegin) >> 2;
    }

    for (std::int32_t index = childCount - 1; index >= 0; --index)
    {
        const std::uint32_t byteOffset =
            static_cast<std::uint32_t>(index) * 4;
        void* child = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->m_childrenBegin), byteOffset));

        const int childHidden =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0xAC / 4])(child, nullptr);

        if (static_cast<std::uint8_t>(childHidden) == 0)
        {
            child = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->m_childrenBegin), byteOffset));

            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x134 / 4])(child, nullptr);
        }
    }

    result = static_cast<int>(self->m_pPointerQuad);
    if (result == 0)
        return result;

    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x268 / 4])(self, nullptr);

    std::uint32_t offsetStorage[2];
    const std::uint32_t* offset =
        reinterpret_cast<std::uint32_t*(__fastcall*)(
            void*, void*, std::uint32_t*)>(
            CSF_AT(void**, self, 0)[0x160 / 4])(
                self, nullptr, offsetStorage);

    const std::uint32_t* origin =
        reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x148 / 4])(self, nullptr);

    const std::uint32_t originY = origin[1];
    const std::uint32_t originX = origin[0];
    const std::uint32_t offsetY = offset[1];
    const std::uint32_t offsetX = offset[0];

    std::uint32_t position[2];
    position[0] = originX + offsetX;
    position[1] = offsetY + originY;

    void* pointerQuad = reinterpret_cast<void*>(self->m_pPointerQuad);
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, pointerQuad, 0)[0x28 / 4])(
            pointerQuad, nullptr, position);

    pointerQuad = reinterpret_cast<void*>(self->m_pPointerQuad);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, pointerQuad, 0)[8 / 4])(pointerQuad, nullptr);
}

HOOK(0x0072AE00, CGuiPanel_Draw, "auto");
