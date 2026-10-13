#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00401F80 : unsigned int __cdecl (const char* a1)  -- ret 0
CSF_FUNC(unsigned int, __cdecl, MeasureCString, 0x00401F80, (const char* a1));

// 0x00429B60 : std::_String_base* __fastcall (this, a2, a3, a4) -- ecx=this, edx=a2, ret 8
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

// CGuiControl method at 0x004381C0 (__thiscall, one stack arg, ret 4).
// Measures the passed C string, then assigns that range into the m_idLeft string member.
// The callee's edx is whatever edx held on entry; the original never sets it, so it is forwarded verbatim.
static void* __fastcall CGuiControl_SetIdLeft(CGuiControl* self, void* edx, char* a2)
{
    unsigned int length = MeasureCString(a2);
    return AssignStringRange_429B60(&self->m_idLeft, edx, a2, length);
}

HOOK(0x4381C0, CGuiControl_SetIdLeft, "auto");
