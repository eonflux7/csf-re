#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address: __cdecl handler taking one int argument (the byte at entry+4).
CSF_FUNC(int, __cdecl, Skin_DispatchType4Handler, 0x00626AC0, (int a1));

// Checks the dword tag at the start of the entry. Tag 4 forwards the byte at +4
// (zero-extended) to the handler and returns its result; any other tag returns the
// entry pointer unchanged. Stdcall, one stack argument (ret 4).
static int __stdcall Skin_ResolveEntryOrDispatch(int a1)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(a1);
    if (CSF_AT(int, entry, 0) == 4)
    {
        return Skin_DispatchType4Handler(static_cast<int>(CSF_AT(uint8_t, entry, 4)));
    }
    return a1;
}

HOOK(0x6C7B20, Skin_ResolveEntryOrDispatch, "auto");
