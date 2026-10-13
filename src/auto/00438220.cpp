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

// 0x00401F80 : unsigned int __cdecl (const char* a1) -- ret 0
CSF_FUNC(unsigned int, __cdecl, MeasureCString, 0x00401F80, (const char* a1));

// 0x00429B60 : std::_String_base* __fastcall (this, a2, a3, a4) -- ecx=this, edx=a2, ret 8
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

// CGuiControl method at 0x00438220 (__thiscall, one stack arg, ret 4).
// Measures the passed C string and assigns it, with its length, to the m_idUp string member
// at +0x6C. The callee's edx is whatever edx held on entry; the original never sets it, so it
// is forwarded verbatim.
static void* __fastcall CGuiControl_SetIdUp(CGuiControl* self, void* edx, char* a2)
{
    unsigned int length = MeasureCString(a2);
    return AssignStringRange_429B60(&self->m_idUp, edx, a2, length);
}

HOOK(0x00438220, CGuiControl_SetIdUp, "auto");
