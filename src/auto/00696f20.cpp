#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));
CSF_FUNC(void*, __fastcall, sub_696CD0, 0x00696CD0, (void* self, int a2, char* a3, unsigned int a4));

static S_004f21d0* __fastcall CoreVariantValues_SetWideString(S_004f21d0* self, void* /*edx*/, wchar_t* text)
{
    size_t textLength = _wcslen(text);
    sub_696CD0(self, 0, reinterpret_cast<char*>(text), static_cast<unsigned int>(textLength));
    return self;
}

HOOK(0x696F20, CoreVariantValues_SetWideString, "auto");
