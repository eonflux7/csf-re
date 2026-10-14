#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original calls).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x539B20: destructor body (member teardown / base-destructor chain) that runs
// after CMenuLoadGameMainMenu restores its vtable pointers.
CSF_FUNC(unsigned int, __fastcall, CMenuLoadGame_DestroyBody, 0x539B20,
         (CMenuLoadGameMainMenu* self, void* edx));

// CMenuLoadGameMainMenu destructor: restores the primary vtable (offset 0x00)
// and the secondary vtable (offset 0x04), runs the destructor body and, when the
// low bit of the flag argument is set, frees the object on the game heap.
// Always returns the object pointer in eax.
static CMenuLoadGameMainMenu* __fastcall CMenuLoadGameMainMenu_Destroy(CMenuLoadGameMainMenu* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x008423D8);
    self->field_0004 = CSF_ADDR_VT(0x00842388);

    CMenuLoadGame_DestroyBody(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x539CB0, CMenuLoadGameMainMenu_Destroy, "auto");
