#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_FindEffectByName(
    CGuiControl* self, void* /*edx*/, const char* name)
{
    void* effect = reinterpret_cast<void*>(self->m_pEffects);
    if (!effect || *name == *CSF_STR(0x0082E65C, ""))
        return 0;

    for (;;)
    {
        const char* effectName =
            reinterpret_cast<const char* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr);
        const char* requestedName = name;

        for (;;)
        {
            const std::uint8_t character =
                static_cast<std::uint8_t>(*effectName);
            if (character != static_cast<std::uint8_t>(*requestedName))
                break;

            if (character == 0)
                return static_cast<int>(
                    reinterpret_cast<std::intptr_t>(effect));

            ++effectName;
            ++requestedName;
        }

        effect = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, effect, 0)[0x10 / 4])(effect, nullptr);
        if (!effect)
            return 0;
    }
}

HOOK(0x00718780, CGuiControl_FindEffectByName, "auto");
