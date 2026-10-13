#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// COptionsFile: stores a2 at +4, then if the global flag is set and the global
// object exists, asks it (vtable slot 0x70) for a sub-object and forwards the
// stored value to that sub-object (vtable slot 0x1C).
static int __fastcall COptionsFile_NotifyOptionChanged(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x4) = value;

    int result = IsGlobalValueNonZero();
    if (static_cast<uint8_t>(result))
    {
        result = GetGlobalValue_70DDE0();
        if (result)
        {
            int globalValue = GetGlobalValue_70DDE0();
            void* globalObject = reinterpret_cast<void*>(globalValue);
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, globalObject, 0)[0x70 / 4])(globalObject, nullptr);
            if (result)
            {
                void* subObject = reinterpret_cast<void*>(result);
                int storedValue = CSF_AT(int, self, 0x4);
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[0x1C / 4])(subObject, nullptr, storedValue);
            }
        }
    }
    return result;
}

HOOK(0x50B330, COptionsFile_NotifyOptionChanged, "auto");
