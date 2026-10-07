#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// CGuiControl::SetSize(const int size[2])
// Stores width = size[0], height = size[1], then notifies the control by
// dispatching vtable slot +0x138 (SendEvent) with id 3, returning its result.
static int __fastcall CGuiControl_SetSize(CGuiControl* self, void* /*edx*/, int* a2)
{
    self->m_width  = static_cast<uint32_t>(a2[0]);
    self->m_height = static_cast<uint32_t>(a2[1]);

    void** vtbl = reinterpret_cast<void**>(self->vtbl);
    // __thiscall SendEvent(int id); this is still held in ecx.
    return (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0x138 / 4]))(self, nullptr, 3);
}

HOOK(0x718050, CGuiControl_SetSize, "auto");
