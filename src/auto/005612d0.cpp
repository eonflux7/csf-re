#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Asks the GUI manager held at +0x40 to resolve the "ListboxPartidas" control
// (virtual factory at vtbl +0xFC, called with the control name) and caches the
// returned control pointer at +0x70.
static int __fastcall CMenuMpSessions_CacheListboxPartidas(CMenuMpSessions* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    void* guiManager = reinterpret_cast<void*>(self->field_0040);
    void** vtbl = *reinterpret_cast<void***>(guiManager);

    int result = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(vtbl[0xFC / 4])(
        guiManager, nullptr, CSF_STR(0x008457D8, "ListboxPartidas"));

    self->field_0070 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x005612D0, CMenuMpSessions_CacheListboxPartidas, "auto");
