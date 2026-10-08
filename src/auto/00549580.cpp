#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

using ResolutionEntries = std::uint32_t[4];
CSF_GLOBAL(ResolutionEntries, g_ResolutionEntries, 0x0089A414);

static int __fastcall CMenuVideoOptions_PopulateResolutionChoices(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_0058);
    if (!control)
        return 0;

    // The conditional push/pop of ESI saves the register, not a stack argument.
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x2B8 / 4])(control, nullptr);

    for (unsigned int index = 0; index < 4; ++index)
    {
        control = reinterpret_cast<void*>(self->field_0058);
        const std::uint32_t entry = g_ResolutionEntries[index];
        ((void (__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, control, 0)[0x29C / 4])(control, nullptr, entry);
    }

    control = reinterpret_cast<void*>(self->field_0058);
    const std::uint32_t selection = self->field_0030;
    return ((int (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, control, 0)[0x2A8 / 4])(control, nullptr, selection);
}

HOOK(0x00549580, CMenuVideoOptions_PopulateResolutionChoices, "auto");
