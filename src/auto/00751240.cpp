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

CSF_FUNC(char, __fastcall, NetChat_OnPrimaryKeyEvent, 0x007511B0, (void* self, void* edx, void* a1));
CSF_FUNC(char, __fastcall, NetChat_OnSecondaryKeyEvent, 0x00750E20, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, NetPlayer_DispatchInheritedEvent, 0x0067F520, (void* self, void* edx, void* a2, int a3, void* a4));

// Routes a keyed event to the primary or secondary handler when the key matches
// one of this object's two stored codes; otherwise defers to the inherited dispatcher.
static char __fastcall NetChat_TryHandleKeyedEvent(NetChat* self, void* /*edx*/, C_00858180* a2, int a3, void* a4)
{
    const uint16_t eventKey = static_cast<uint16_t>(a3);

    if (eventKey == self->field_0020)
    {
        NetChat_OnPrimaryKeyEvent(a2, nullptr, a4);
        return 1;
    }
    if (eventKey == self->field_0022)
    {
        NetChat_OnSecondaryKeyEvent(a2, nullptr, a4);
        return 1;
    }
    return NetPlayer_DispatchInheritedEvent(self, nullptr, a2, a3, a4);
}

HOOK(0x00751240, NetChat_TryHandleKeyedEvent, "auto");
