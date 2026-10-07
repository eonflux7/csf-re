#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiControl_AddEffect(
    CGuiControl* self, void* /*edx*/, void* effect)
{
    if (!effect)
        return;

    void* current = reinterpret_cast<void*>(self->m_pEffects);
    if (!current)
    {
        self->m_pEffects = reinterpret_cast<uint32_t>(effect);
        return;
    }

    void* last;
    do
    {
        last = current;
        current = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, current, 0)[4])(current, nullptr);
    }
    while (current);

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, last, 0)[3])(last, nullptr, effect);
}

HOOK(0x00718800, CGuiControl_AddEffect, "auto");
