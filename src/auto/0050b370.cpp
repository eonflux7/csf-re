#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// COptionsFile method: stores a value at +0x08, then forwards it to the
// global object's notify slot (vtbl+0x10) and to the object returned by
// the global's slot (vtbl+0x70) through its slot (vtbl+0x24).
static int __fastcall OptionsFile_StoreValueAndNotify(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x8) = value;

    int result = IsGlobalValueNonZero();
    if ((result & 0xFF) != 0)
    {
        result = GetGlobalValue_70DDE0();
        if (result != 0)
        {
            int globalObject = GetGlobalValue_70DDE0();
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, reinterpret_cast<void*>(globalObject), 0)[4])(
                reinterpret_cast<void*>(globalObject), nullptr, CSF_AT(int, self, 0x8));

            int globalObjectAgain = GetGlobalValue_70DDE0();
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(globalObjectAgain), 0)[28])(
                reinterpret_cast<void*>(globalObjectAgain), nullptr);

            if (result != 0)
            {
                return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, reinterpret_cast<void*>(result), 0)[9])(
                    reinterpret_cast<void*>(result), nullptr, CSF_AT(int, self, 0x8));
            }
        }
    }
    return result;
}

HOOK(0x0050B370, OptionsFile_StoreValueAndNotify, "auto");
