#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_PrepareListRendering,
         0x00719A20, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_UpdateSelectionDisplay,
         0x007156B0, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiListBoxBase_PrepareVisibleRows,
         0x007179C0, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiListBoxBase_SnapHeightToRowIncrement,
         0x00714480, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_DrawHeader,
         0x00714510, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_LayoutColumnHeaders,
         0x00714E80, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_DrawVisibleCells,
         0x007150A0, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_RenderVisibleRowHighlights,
         0x00714A60, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_HandleSliderOverflow,
         0x00714640, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiListBoxBase_FinalizeDisplayUpdate,
         0x00715270, (CGuiListBoxBase* self, void* edx));

// EBX and EDI carry inherited rendering contexts, not stack arguments.
// Disable optimization to avoid allocating persistent values or hoisted
// call targets to those registers during the rendering sequence.
static __declspec(noinline) __attribute__((optnone))
char __fastcall CGuiListBoxBase_UpdateAndRender(
    CGuiListBoxBase* self, void* /*edx*/)
{
    const char skipUpdate = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr));

    if (skipUpdate != 0)
        return skipUpdate;

    CGuiListBoxBase_PrepareListRendering(self, nullptr);

    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    std::uint32_t scrollPosition;
    if (slider != nullptr)
    {
        scrollPosition = static_cast<std::uint32_t>(
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, slider, 0)[0x270 / 4])(slider, nullptr));
    }
    else
    {
        scrollPosition = self->field_0168;
    }

    self->field_0168 = scrollPosition;
    CGuiListBoxBase_UpdateSelectionDisplay(self, nullptr);
    CGuiListBoxBase_PrepareVisibleRows(self, nullptr);
    CGuiListBoxBase_SnapHeightToRowIncrement(self, nullptr);

    const std::uint32_t layoutValue = self->field_0138;
    if (layoutValue != 0)
    {
        ((void(__fastcall*)(void*, void*, std::uint32_t))
             CSF_AT(void**, self, 0)[0x238 / 4])(
                 self, nullptr, layoutValue);
    }

    CGuiListBoxBase_DrawHeader(self, nullptr);
    CGuiListBoxBase_LayoutColumnHeaders(self, nullptr);
    CGuiListBoxBase_DrawVisibleCells(self, nullptr);
    CGuiListBoxBase_RenderVisibleRowHighlights(self, nullptr);
    CGuiListBoxBase_HandleSliderOverflow(self, nullptr);
    return CGuiListBoxBase_FinalizeDisplayUpdate(self, nullptr);
}

HOOK(0x00717D00, CGuiListBoxBase_UpdateAndRender, "auto");
