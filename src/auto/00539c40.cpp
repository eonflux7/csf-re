#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original destructor uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x539B20: destructor body / member teardown of the CMenuLoadGame chain; runs after
// the CMenuLoadGameInGame destructor restores the vtable pointers, before the free.
CSF_FUNC(void, __fastcall, CMenuLoadGameInGame_CleanUp, 0x539B20,
         (CMenuLoadGameInGame* self, void* edx));

// CMenuLoadGameInGame destructor: restores the primary vtable (offset 0x00) and the
// secondary vtable (offset 0x04), runs the destructor body and, when the low bit of the
// flag argument is set, frees the object on the game heap. Always returns the object
// pointer in eax.
static void* __fastcall CMenuLoadGameInGame_Destroy(CMenuLoadGameInGame* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = 0x00842328u;
    self->field_0004 = 0x008422D8u;

    CMenuLoadGameInGame_CleanUp(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x539C40, CMenuLoadGameInGame_Destroy, "auto");

