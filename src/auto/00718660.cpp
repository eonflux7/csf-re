#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CGuiSlider method at 0x718660 (ret 0, __thiscall).
//
// Disassembly:
//   parent = this->vtbl[0x10C / 4](this);   // virtual getter, called twice by the original
//   if (parent != 0)
//       return parent->vtbl[0x158 / 4](parent, this);
//   return 0;                               // first call's (null) result forwarded unchanged
//
// The virtual table pointer at offset 0 is not a declared field, so it is read
// through CSF_AT as prescribed. The first virtual is invoked twice on purpose
// (the original re-reads the vtable and re-calls it), no result is cached.
static int __fastcall CGuiSlider_ForwardSelfToParent(CGuiSlider* self, void* /*edx*/)
{
    if (((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x10C / 4])(self, nullptr))
    {
        void* parent = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x10C / 4])(self, nullptr);
        // ecx = parent, one stack argument (self): __thiscall callee -> __fastcall with edx unused
        return ((int (__fastcall*)(void*, void*, void*))CSF_AT(void**, parent, 0)[0x158 / 4])(parent, nullptr, self);
    }
    return 0;
}

HOOK(0x718660, CGuiSlider_ForwardSelfToParent, "auto");
