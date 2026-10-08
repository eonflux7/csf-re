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

// 0x0055DDA0: handler invoked for event id 13. It receives `this` in ecx, the
// event pointer forwarded in edx, and the same event pointer as the single
// stack argument (ret 4). Bound as __fastcall so the caller's ecx/edx and the
// stack slot line up with the original call site.
CSF_FUNC(int, __fastcall, CMenuPlayerList_HandleEvent13, 0x0055DDA0, (CMenuPlayerList* self, void* edx, int event));

// 0x0055DEE0 - Event dispatcher for CMenuPlayerList (ListaJugadores).
// Loads the event id pointed to by the stack argument; when the id is 13 it
// forwards to the id-13 handler, otherwise it returns (id - 13).
static int __fastcall CMenuPlayerList_HandleMenuEvent(CMenuPlayerList* self, void* /*edx*/, int* event)
{
    int result = *event - 13;
    if (result == 0)
        return CMenuPlayerList_HandleEvent13(self, event, reinterpret_cast<int>(event));
    return result;
}

HOOK(0x0055DEE0, CMenuPlayerList_HandleMenuEvent, "auto");
