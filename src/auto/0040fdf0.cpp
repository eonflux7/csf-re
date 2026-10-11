#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x41E110: __fastcall with (self, edx), returns int in eax.
CSF_FUNC(int, __fastcall, C_0082deb8_FinalizeBaseAfterChild, 0x0041E110, (void* self, void* edx));

// Destroys the owned child (slot 0 of its vtable, called with free flag 1), clears the
// pointer, then finalizes the base part of the object. Returns the base finalizer's result.
static int __fastcall C_0082deb8_ReleaseChildAndFinalize(C_0082deb8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082DEB8);

    void* ownedChild = reinterpret_cast<void*>(self->field_0010);
    if (ownedChild) {
        // Virtual slot 0 of the child's vtable, thiscall with one argument (1).
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedChild, 0)[0])(ownedChild, nullptr, 1);
        self->field_0010 = 0;
    }

    int result = C_0082deb8_FinalizeBaseAfterChild(self, nullptr);
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0040FDF0, C_0082deb8_ReleaseChildAndFinalize, "auto");
