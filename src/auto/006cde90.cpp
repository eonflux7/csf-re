#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor of C_008634c8 (__fastcall: ecx=this, edx unused).
CSF_FUNC(C_008634c8*, __fastcall, C_008634c8_Constructor, 0x00708940, (C_008634c8* self, void*));

static C_008634c8* __cdecl MissionSetupLoader_Create()
{
    C_008634c8* loader = reinterpret_cast<C_008634c8*>(Mem_Alloc(4));
    if (!loader)
        return nullptr;

    C_008634c8_Constructor(loader, nullptr);
    *(void***)loader = CSF_VTBL(0x00862944);
    return loader;
}

HOOK(0x006CDE90, MissionSetupLoader_Create, "auto");
