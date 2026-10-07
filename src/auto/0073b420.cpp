#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00401F80 : unsigned int __cdecl (const char* a1)  -- ret 0
CSF_FUNC(unsigned int, __cdecl, MeasureCString, 0x401F80, (const char* a1));

// 0x00429B60 : std::_String_base* __fastcall (this, a2, a3, a4) -- ecx=this, edx=a2, ret 8
CSF_FUNC(void*, __fastcall, AssignStringValue, 0x429B60,
         (void* thisptr, int a2, char* a3, unsigned int a4));

// C_00867444 method at 0x0073B420 (__thiscall, one stack arg, ret 4).
// Measures the passed C string and forwards it, together with the (unsigned) result, to the
// string object embedded at this+0x0C. The callee's second argument is whatever edx held on
// entry; the original never sets it, so it is forwarded verbatim.
static void* __fastcall C_00867444_AssignStringFromCStr(C_00867444* self, void* edx, char* a2)
{
    unsigned int length = MeasureCString(a2);
    return AssignStringValue(reinterpret_cast<uint8_t*>(self) + 0x0C,
                             static_cast<int>(reinterpret_cast<uintptr_t>(edx)),
                             a2,
                             length);
}

HOOK(0x73B420, C_00867444_AssignStringFromCStr, "auto");
