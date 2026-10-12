#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __fastcall, HudStringShow_SetSelectionData, 0x004F4850,
               (void* self, void* edx, void* a2));

static void* __fastcall HudStringShow_ForwardSelectionData(
    void* self, void* /*edx*/, void* selectionData)
{
    return HudStringShow_SetSelectionData(
        CSF_AT(void*, self, 0x0C), nullptr, selectionData);
}

HOOK(0x004F49E0, HudStringShow_ForwardSelectionData, "auto");
