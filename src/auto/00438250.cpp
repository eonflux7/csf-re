#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00401F80 : unsigned int __cdecl (const char* a1)  -- ret 0
CSF_FUNC(unsigned int, __cdecl, MeasureCString, 0x401F80, (const char* a1));

// 0x00429B60 : std::_String_base* __fastcall (this, a2, a3, a4) -- ecx=this, edx=a2, ret 8
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x429B60,
         (void* thisptr, int a2, char* a3, unsigned int a4));

// CGuiControl method at 0x00438250 (__thiscall, one stack arg, ret 4).
// Measures the passed C string and assigns it, with the measured length, into the
// string member m_idDown. The callee's second argument is whatever edx held on entry;
// the original never sets it, so it is forwarded verbatim.
static void* __fastcall CGuiControl_SetIdDown(CGuiControl* self, void* edx, char* a2)
{
    unsigned int length = MeasureCString(a2);
    return AssignStringRange_429B60(reinterpret_cast<void*>(&self->m_idDown),
                                    static_cast<int>(reinterpret_cast<uintptr_t>(edx)),
                                    a2,
                                    length);
}

HOOK(0x438250, CGuiControl_SetIdDown, "auto");
