#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Stores the incoming value in the options object, then, if the global gate is
// open and the global object exists, forwards the value to that object's
// vtable slot 26 (+0x68) and returns that call's result.
static int __fastcall OptionsFile_StoreAndForwardValue(void* self, void* /*edx*/, int a2)
{
    CSF_AT(int, self, 0xC) = a2;

    int result = IsGlobalValueNonZero();
    if (static_cast<uint8_t>(result) != 0)
    {
        result = GetGlobalValue_70DDE0();
        if (result != 0)
        {
            int globalObjectValue = GetGlobalValue_70DDE0();
            void* globalObject = reinterpret_cast<void*>(
                static_cast<uintptr_t>(static_cast<uint32_t>(globalObjectValue)));
            int storedValue = CSF_AT(int, self, 0xC);
            auto setValueFn = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, globalObject, 0)[0x68 / 4]);
            return setValueFn(globalObject, nullptr, storedValue);
        }
    }
    return result;
}

HOOK(0x0050B3C0, OptionsFile_StoreAndForwardValue, "auto");
