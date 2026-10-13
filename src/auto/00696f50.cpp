#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(S_004f21d0*, __fastcall, AssignWideStringBuffer, 0x00696E00,
         (S_004f21d0* self, int a2, uint16_t* a3, uint32_t a4));

CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

// Original push order for 0x00696E00: String last on top? No: push eax (len),
// then push edi (String), so arg1 = String, arg2 = length.
static S_004f21d0* __fastcall ConstructWideString(S_004f21d0* self, void* /*edx*/, wchar_t* String)
{
    self->field_0018 = 7;
    self->field_0014 = 0;
    self->field_0004 = 0;

    size_t length = _wcslen(String);
    AssignWideStringBuffer(self, 0,
                           reinterpret_cast<uint16_t*>(String),
                           static_cast<uint32_t>(length));
    return self;
}

HOOK(0x00696F50, ConstructWideString, "auto");
