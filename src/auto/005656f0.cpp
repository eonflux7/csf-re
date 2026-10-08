#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Dispatches an Enter key press (message id 13) to the network-type button
// that matches the focused child control. Flags the matching button as pressed,
// then forwards the event to the screen's base handler (vtable slot 18).
static void __fastcall CMenuMpNetworkType_HandleEnterKey(CMenuMpNetworkType* self, void* /*edx*/, uint32_t* event)
{
    if (*event != 13)
        return;

    // Ask the child control (stored at +0x38) which item is currently selected.
    // Virtual slot 65 (+0x104) takes no arguments; the child is passed as ecx.
    void* child = reinterpret_cast<void*>(self->field_0038);
    uint32_t selectedItem = static_cast<uint32_t>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[65])(child, nullptr));

    if (selectedItem == self->field_0014)
    {
        self->field_0024 = 1;
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[18])(self, nullptr, event);
        return;
    }

    if (selectedItem == self->field_0018)
    {
        self->field_0025 = 1;
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[18])(self, nullptr, event);
        return;
    }

    if (self->field_001c != 0 && selectedItem == self->field_001c)
    {
        self->field_0026 = 1;
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[18])(self, nullptr, event);
        return;
    }

    if (self->field_0020 != 0 && selectedItem == self->field_0020)
    {
        self->field_0027 = 1;
    }

    ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[18])(self, nullptr, event);
}

HOOK(0x5656F0, CMenuMpNetworkType_HandleEnterKey, "auto");
