#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_globalValue_8B0560, 0x008B0560);

static C_0085ec18* __cdecl MenuControls_ClearSelectionState()
{
    auto* current = reinterpret_cast<C_0085ec18*>(g_globalValue_8B0560);
    auto* original = current;
    void* selection = CSF_AT(void*, current, 0x18C);
    auto* selectionStorage = reinterpret_cast<std::uint8_t*>(current) + 0x18C;

    if (selection)
    {
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, selection, 0)[0])(selection, nullptr, 1);
        CSF_AT(std::uint32_t, selectionStorage, 0) = 0;
        current = reinterpret_cast<C_0085ec18*>(g_globalValue_8B0560);
    }

    void* secondary = CSF_AT(void*, original, 0x190);
    auto* result = reinterpret_cast<C_0085ec18*>(secondary);
    if (secondary)
    {
        result = ((C_0085ec18*(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, secondary, 0)[0])(secondary, nullptr, 1);
        CSF_AT(std::uint32_t, original, 0x190) = 0;
        current = reinterpret_cast<C_0085ec18*>(g_globalValue_8B0560);
    }

    if (current)
    {
        result = (C_0085ec18*)current->vSlot0(1);
        g_globalValue_8B0560 = 0;
    }

    return result;
}

HOOK(0x004FABB0, MenuControls_ClearSelectionState, "auto");
