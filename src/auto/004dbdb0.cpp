// Recovered from 0x004DBDB0 (C_0083dfa0 destructor body, module ai_state).
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address, signatures copied from the packet).
CSF_FUNC(int, __fastcall, sub_4DBCF0, 0x004DBCF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destroys the base-agent sub-object (embedded at this+0x8) and then the
// object itself, resetting every vtable it passes through to C_0082d410.
static int __fastcall C_0083dfa0_DestroyBaseAgent(C_0083dfa0* self, void* /*edx*/)
{
    // v2 = &this->field_0008 (embedded sub-object, esi in the original)
    uint8_t* v2 = reinterpret_cast<uint8_t*>(self) + 8;

    self->vtbl = CSF_VTBL(0x0083DFA0);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x0083DF18));

    sub_4DBCF0(v2, nullptr);

    int result = static_cast<int>(CSF_AT(uint32_t, v2, 0x14));
    if (result) {
        CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, v2, 0x14))));
        CSF_AT(uint32_t, v2, 0x14) = 0;
    }

    void* v4 = CSF_AT(void*, v2, 0xC);
    CSF_AT(void*, v2, 0) = CSF_VTBL(0x0083DEF8);
    if (v4) {
        // vDestroy(1): slot 0 of the object's vtable.
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, v4, 0)[0])(v4, nullptr, 1);
        CSF_AT(uint32_t, v2, 0xC) = 0;
    }

    CSF_AT(void*, v2, 0) = CSF_VTBL(0x0082D410);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004DBDB0, C_0083dfa0_DestroyBaseAgent, "auto");
