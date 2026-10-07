#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_FindEffectById(
    CGuiControl* self, void* /*edx*/, int effectId)
{
    void* effect = reinterpret_cast<void*>(self->m_pEffects);
    if (!effect || effectId == -1)
        return 0;

    while (((int(__fastcall*)(void*, void*))CSF_AT(void**, effect, 0)[1])(
               effect, nullptr) != effectId)
    {
        effect = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, effect, 0)[4])(
            effect, nullptr);
        if (!effect)
            return 0;
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(effect));
}

HOOK(0x00718740, CGuiControl_FindEffectById, "auto");
