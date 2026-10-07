#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// CGuiControl::ClearListeners - frees the listener vector's backing buffer and resets its three pointers.
static int __fastcall CGuiControl_ClearListeners(CGuiControl* self, void* /*edx*/)
{
    int result = static_cast<int>(self->m_listenersBegin);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->m_listenersBegin));

    self->m_listenersBegin = 0;
    self->m_listenersEnd = 0;
    self->m_listenersCap = 0;
    return result;
}

HOOK(0x00719D10, CGuiControl_ClearListeners, "auto");
