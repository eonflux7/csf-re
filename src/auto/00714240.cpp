// CGuiListBoxBase (0x714240)
// Guarded update: run the derived update only when two boolean virtual
// predicates (vtable slots 0x6c/4 and 0xbc/4) both report true.
// The predicate result itself is returned otherwise (full EAX of the last call).
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00718700 - BOOL __thiscall sub_718700(_BYTE *this), implemented elsewhere.
CSF_FUNC(int, __fastcall, Sub_718700, 0x718700, (void* self, void* edx));

static int __fastcall CGuiListBoxBase_UpdateIfEnabled(CGuiListBoxBase* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    // mov eax, [esi] ; call dword ptr [eax + 0x6c]   (ecx already == this)
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x6c / 4])(self, nullptr);
    if ((uint8_t)result)
    {
        // mov edx, [esi] ; mov ecx, esi ; call dword ptr [edx + 0xbc]
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xbc / 4])(self, nullptr);
        if ((uint8_t)result)
        {
            // mov ecx, esi ; call 0x718700
            return Sub_718700(self, nullptr);
        }
    }
    return result;
}

HOOK(0x714240, CGuiListBoxBase_UpdateIfEnabled, "auto");
