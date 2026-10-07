#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiTextureListBox_SetScrollPosition(
    CGuiTextureListBox* self, void* /*edx*/, int position)
{
    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider)
    {
        using SetPosition = void (__fastcall*)(void*, void*, int, int);
        reinterpret_cast<SetPosition>(
            CSF_AT(void**, slider, 0)[0x26c / 4])(
                slider, nullptr, position, 0);

        slider = reinterpret_cast<void*>(self->m_pSlider);
        using GetPosition = int (__fastcall*)(void*, void*);
        self->field_0168 = reinterpret_cast<GetPosition>(
            CSF_AT(void**, slider, 0)[0x270 / 4])(
                slider, nullptr);
    }
    else
    {
        self->field_0168 = static_cast<std::uint32_t>(position);
    }
}

HOOK(0x007140B0, CGuiTextureListBox_SetScrollPosition, "auto");
