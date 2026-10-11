#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializer for the freshly allocated skin object (__fastcall: ecx=this, edx unused).
CSF_FUNC(uint32_t*, __fastcall, sub_6C7EA0, 0x006C7EA0, (void* self, void* edx));

static uint32_t* __cdecl GfxSkin_AllocAndConstruct()
{
    void* skinObject = Mem_Alloc(36);
    if (skinObject)
        return sub_6C7EA0(skinObject, nullptr);
    return nullptr;
}

HOOK(0x006C8370, GfxSkin_AllocAndConstruct, "auto");
