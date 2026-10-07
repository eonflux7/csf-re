#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// int __cdecl RtDictGetSchema(int a1)
CSF_FUNC(int, __cdecl, RtDictGetSchema, 0x7FDAD0, (int a1));

// Returns the schema of the runtime dictionary held at this+0x3c.
static int __fastcall GetRtDictSchema(void* self, void* /*edx*/)
{
    return RtDictGetSchema(CSF_AT(uint32_t, self, 0x3c));
}

HOOK(0x006D6360, GetRtDictSchema, "auto");
