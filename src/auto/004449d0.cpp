#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (__cdecl, caller pops nothing extra: the call site does add esp, 4).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// CMissionScript constructor (__fastcall: ecx = this, edx unused).
CSF_FUNC(CMissionScript*, __fastcall, CMissionScript_Construct, 0x0046ADA0, (CMissionScript* self, void*));

// C_008310b8 constructor: sets its vtable, clears two flag bytes, and allocates
// the 212-byte mission script object that it owns through field_0004.
static C_008310b8* __fastcall C_008310b8_Construct(C_008310b8* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x008310B8);
    CSF_AT(uint8_t, self, 0x8) = 0;
    CSF_AT(uint8_t, self, 0x9) = 0;

    void* missionScriptMemory = Mem_Alloc(212);
    if (missionScriptMemory)
    {
        CMissionScript* missionScript =
            CMissionScript_Construct(reinterpret_cast<CMissionScript*>(missionScriptMemory), nullptr);
        self->field_0004 = (uint32_t)missionScript;
    }
    else
    {
        self->field_0004 = 0;
    }
    return self;
}

HOOK(0x004449D0, C_008310b8_Construct, "auto");
