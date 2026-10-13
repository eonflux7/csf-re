#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall AiStateAccessors_DispatchLinkedTarget(void* self, void* /*edx*/, int argument)
{
    void* linkedTarget = CSF_AT(void*, self, 0x08);
    if (!linkedTarget)
        return 0;

    void* resolver = CSF_AT(void*, self, 0x10);
    void** resolverVtable = CSF_AT(void**, resolver, 0);
    void* receiver = CSF_AT(void*, linkedTarget, 0x10);
    void** receiverVtable = CSF_AT(void**, receiver, 0);

    int resolvedArgument =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            resolverVtable[0x1C / 4])(resolver, nullptr, argument);

    return reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
        receiverVtable[0x80 / 4])(
            receiver, nullptr, CSF_AT(void*, self, 0x08), resolvedArgument);
}

HOOK(0x004DBCB0, AiStateAccessors_DispatchLinkedTarget, "auto");
