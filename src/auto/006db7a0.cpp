#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Base-class initializer for the 96-byte render-engine object (__fastcall: ecx=this, edx unused).
CSF_FUNC(int, __fastcall, sub_6DBCD0, 0x006DBCD0, (void* self, void* edx));

// Allocates a 96-byte block from the game heap, runs the base initializer, installs the
// C_00861460 vtable and clears the trailing dword at +0x5C. Returns nullptr if allocation fails.
// The original reaches the init/store tail through a direct jmp to 0x006DC1D0; that body is inlined here.
static void* __cdecl RenderEngine_AllocAndInitResourceScreen()
{
    void* engineObject = Mem_Alloc(0x60);
    if (!engineObject)
        return nullptr;

    sub_6DBCD0(engineObject, nullptr);
    *(void***)engineObject = CSF_VTBL(0x00861460);
    CSF_AT(int, engineObject, 0x5C) = 0;
    return engineObject;
}

HOOK(0x006DB7A0, RenderEngine_AllocAndInitResourceScreen, "auto");
