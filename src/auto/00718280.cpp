#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00718280 (20 bytes) — CGuiControl::SendEvent(int id)
// Thin wrapper: forwards (id, this) to the virtual FireEvent handler living at
// vtable slot +0x218. The two arguments are pushed in order (this, id), and the
// callee pops both of them (__stdcall), so this function is a pure forwarder.
static int __fastcall CGuiControl_SendEvent(CGuiControl* self, void* /*edx*/, int eventId)
{
    typedef int (__stdcall *CGuiControl_FireEventFn)(int eventId, CGuiControl* sender);
    return reinterpret_cast<CGuiControl_FireEventFn>(self->vtbl[0x218 / 4])(eventId, self);
}

HOOK(0x718280, CGuiControl_SendEvent, "auto");
