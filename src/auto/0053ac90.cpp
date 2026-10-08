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

// Dispatches a click message to the matching option button (juego, sonido, video).
// When the message's source id matches a button, it notifies the owning parent
// (vtable slot 0x60) and then runs the button's own handler (slot 0x50 / 0x54 / 0x58),
// finally forwarding to the base handler (slot 0x48). Otherwise it falls through.
static int __fastcall CMenuOptions_RouteButtonClick(CMenuOptions* self, void* /*edx*/, int a2)
{
    void* clickMessage = reinterpret_cast<void*>(a2);

    uint32_t gameButtonId = self->field_0028;
    if (gameButtonId != 0 && gameButtonId == CSF_AT(uint32_t, clickMessage, 4)) {
        void* parentObject = reinterpret_cast<void*>(self->field_0014);
        ((void(__fastcall*)(void*, void*, void*, int))(CSF_AT(void**, parentObject, 0)[0x60 / 4]))(
            parentObject, nullptr, self, 0);
        ((void(__fastcall*)(void*, void*))(CSF_AT(void**, self, 0)[0x50 / 4]))(self, nullptr);
        return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, self, 0)[0x48 / 4]))(self, nullptr, a2);
    }

    uint32_t soundButtonId = self->field_0024;
    if (soundButtonId != 0 && soundButtonId == CSF_AT(uint32_t, clickMessage, 4)) {
        void* parentObject = reinterpret_cast<void*>(self->field_0014);
        ((void(__fastcall*)(void*, void*, void*, int))(CSF_AT(void**, parentObject, 0)[0x60 / 4]))(
            parentObject, nullptr, self, 0);
        ((void(__fastcall*)(void*, void*))(CSF_AT(void**, self, 0)[0x54 / 4]))(self, nullptr);
        return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, self, 0)[0x48 / 4]))(self, nullptr, a2);
    }

    uint32_t videoButtonId = self->field_002c;
    if (videoButtonId != 0 && videoButtonId == CSF_AT(uint32_t, clickMessage, 4)) {
        ((void(__fastcall*)(void*, void*))(CSF_AT(void**, self, 0)[0x58 / 4]))(self, nullptr);
    }
    return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, self, 0)[0x48 / 4]))(self, nullptr, a2);
}

HOOK(0x0053AC90, CMenuOptions_RouteButtonClick, "auto");
