#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards a 4-byte-argument update to the embedded sub-object at +0x24 and to every
// child pointer in the array at +0xAC (count at +0xB4), unless the global object flag
// is set or the skip flag at +0xCC is set. Returns the low byte of the last value
// the original leaves in eax.
static char __fastcall PTank_ForwardToChildren(C_00862240* self, void* /*edx*/, int a2)
{
    const char globalFlag = self->vCheckGlobalObjectFlag();
    if (globalFlag)
        return globalFlag;

    // Embedded sub-object at +0x24: its first dword is its vtable pointer.
    void* embeddedChild = self->field_0024 == 0 ? nullptr : reinterpret_cast<void*>(&self->field_0024);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, embeddedChild, 0)[12])(embeddedChild, nullptr, a2);

    if (self->field_00cc)
        return static_cast<char>(self->field_00cc);

    for (uint32_t i = 0; i < self->field_00b4; ++i)
    {
        uint32_t childValue = reinterpret_cast<uint32_t*>(self->field_00ac)[i];
        void* child = reinterpret_cast<void*>(childValue);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[12])(child, nullptr, a2);
    }

    return static_cast<char>(self->field_00b4);
}

HOOK(0x006EFEF0, PTank_ForwardToChildren, "auto");
