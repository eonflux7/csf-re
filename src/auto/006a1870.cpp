#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: __cdecl, 3 stack args, caller cleans up (add esp, 0xC). Return value unused by the caller.
CSF_FUNC(int, __cdecl, ZlibLog_ApplyStreamState, 0x6A4DF0, (void* state, void* owner, int flag));

// Resets the log/stream context at `ctx`. Returns -2 for a null context or a null stream,
// otherwise clears the context counters, sets the stream state word to 7 when its
// secondary field is nonzero (else 0), applies the state, and returns 0.
static int __cdecl ZlibLog_ResetStream(void* ctx)
{
    if (!ctx)
        return -2;

    void* stream = CSF_AT(void*, ctx, 0x1C);
    if (!stream)
        return -2;

    CSF_AT(int, ctx, 0x14) = 0;
    CSF_AT(int, ctx, 0x08) = 0;
    CSF_AT(int, ctx, 0x18) = 0;

    CSF_AT(int, stream, 0x00) = CSF_AT(int, stream, 0x0C) != 0 ? 7 : 0;

    ZlibLog_ApplyStreamState(CSF_AT(void*, CSF_AT(void*, ctx, 0x1C), 0x14), ctx, 0);
    return 0;
}

HOOK(0x6A1870, ZlibLog_ResetStream, "auto");
