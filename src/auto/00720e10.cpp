#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_Ctor, 0x0071A010,
         (void* self, void* edx, int owner, int argument));

CSF_GLOBAL(void*, g_GuiStaticVtable, 0x00864BC0);

static CGuiStatic* __fastcall CGuiStatic_Construct(
    CGuiStatic* self, void* /*edx*/, int owner, int argument)
{
    CGuiControl_Ctor(self, nullptr, owner, argument);

    CSF_AT(void**, self, 0x00) = &g_GuiStaticVtable;
    self->field_0138 = 0;
    self->m_pText = 0;
    self->field_0140 = 1.0f;
    self->field_0144 = 0;
    self->field_0148 = 0.0f;
    self->field_0150 = 0;
    self->field_014c = 0;
    self->m_bBlink = 0;

    void* ownerObject = reinterpret_cast<void*>(owner);
    void* quad =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerObject, 0x00)[0x48 / 4])(
                ownerObject, nullptr);

    self->m_pQuad = reinterpret_cast<std::uintptr_t>(quad);

    std::uint32_t initialValues[4] = {0, 0, 0, 0};
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, quad, 0x00)[0x2C / 4])(
            quad, nullptr, initialValues);

    self->field_015c = 0xFFFFFFFFu;
    self->field_0158 = 0xFFFFFFFFu;
    return self;
}

HOOK(0x00720E10, CGuiStatic_Construct, "auto");
