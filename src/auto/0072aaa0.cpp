#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_UpdateMouseHover, 0x00718DE0,
         (CGuiControl* self, void* edx, std::int32_t* position, char inputState));

static int __fastcall CGuiPanel_UpdateMouseHover(
    CGuiPanel* self, void* /*edx*/,
    std::int32_t* mousePosition, int inputState)
{
    const std::uint32_t childrenBegin = self->field_0138;
    std::int32_t childCount = 0;
    if (childrenBegin != 0) {
        childCount =
            static_cast<std::int32_t>(self->field_013c - childrenBegin) >> 2;
    }

    std::uint32_t panelPositionBuffer[2];
    std::uint32_t childPositionBuffer[2];
    std::uint32_t translatedPosition[2];

    for (std::int32_t index = childCount - 1; index >= 0; --index) {
        void* child =
            reinterpret_cast<void**>(self->field_0138)[index];

        auto* panelPosition =
            ((std::uint32_t* (__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, panelPositionBuffer);

        const std::uint32_t panelX = panelPosition[0];
        const std::uint32_t panelY = panelPosition[1];
        const std::uint32_t mouseY =
            static_cast<std::uint32_t>(mousePosition[1]);
        const std::uint32_t mouseX =
            static_cast<std::uint32_t>(mousePosition[0]);
        const std::uint32_t absoluteY = panelY + mouseY;
        const std::uint32_t absoluteX = panelX + mouseX;

        auto* childPosition =
            ((std::uint32_t* (__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, child, 0)[0x160 / 4])(
                    child, nullptr, childPositionBuffer);

        const std::uint32_t childY = childPosition[1];
        const std::uint32_t childX = childPosition[0];
        translatedPosition[0] = absoluteX - childX;
        translatedPosition[1] = absoluteY - childY;

        ((void (__fastcall*)(void*, void*, std::uint32_t*, int))
            CSF_AT(void**, child, 0)[0x14C / 4])(
                child, nullptr, translatedPosition, inputState);
    }

    if (self->m_pPointerQuad != 0) {
        auto* panelPosition =
            ((std::uint32_t* (__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, childPositionBuffer);

        const std::uint32_t panelY = panelPosition[1];
        const std::uint32_t mouseY =
            static_cast<std::uint32_t>(mousePosition[1]);
        const std::uint32_t panelX = panelPosition[0];
        const std::uint32_t absoluteY = panelY + mouseY;
        const std::uint32_t mouseX =
            static_cast<std::uint32_t>(mousePosition[0]);

        translatedPosition[0] = mouseX + panelX;
        translatedPosition[1] = absoluteY;

        void* pointerQuad =
            reinterpret_cast<void*>(self->m_pPointerQuad);
        ((void (__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, pointerQuad, 0)[0x28 / 4])(
                pointerQuad, nullptr, translatedPosition);
    }

    return CGuiControl_UpdateMouseHover(
        self, nullptr, mousePosition, static_cast<char>(inputState));
}

HOOK(0x0072AAA0, CGuiPanel_UpdateMouseHover, "auto");
