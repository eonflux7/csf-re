#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (original addresses), copied from the packet.
CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));
CSF_FUNC(char, __stdcall, NetGameDDL_ProcessBaseField, 0x0065E740, (int a1, int a2));

// Dispatches a field operation on a NetPlayerList DDL object. The original
// (stdcall, 3 args, ret 12) first forwards to the embedded NetPlayer handler
// when a2 is null, then compares a2 against a1 + 0x44 (the embedded NetPlayer
// sub-object): on a match it returns the embedded handler's result, otherwise
// it returns the base NetGame handler's result.
static int __stdcall NetPlayerListDDL_DispatchField(int a1, int a2, int a3)
{
    if (a2 == 0)
        NetPlayerDDL_ProcessEmbeddedField(a3);

    if (a2 == a1 + 0x44)
        return NetPlayerDDL_ProcessEmbeddedField(a3);

    return NetGameDDL_ProcessBaseField(a2, a3);
}

HOOK(0x0074F3D0, NetPlayerListDDL_DispatchField, "auto");
