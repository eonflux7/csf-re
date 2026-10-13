#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"


CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x00719B10,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiStatic_Destroy(CGuiStatic* self, void* /*edx*/)
{
    const std::uint32_t resource = self->field_0138;
    CSF_AT(void**, self, 0) =
        CSF_VTBL(0x00864BC0);

    if (resource)
    {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(resource) + 4, nullptr);
        self->field_0138 = 0;
    }

    void* text = reinterpret_cast<void*>(self->m_pText);
    if (text)
    {
        auto destroy = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, text, 0)[0]);
        destroy(text, nullptr, 1);
        self->m_pText = 0;
    }

    void* quad = reinterpret_cast<void*>(self->m_pQuad);
    if (quad)
    {
        auto destroy = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, quad, 0)[0]);
        destroy(quad, nullptr, 1);
        self->m_pQuad = 0;
    }

    return CGuiControl_Destroy(static_cast<CGuiControl*>(self), nullptr);
}

HOOK(0x007209F0, CGuiStatic_Destroy, "auto");
