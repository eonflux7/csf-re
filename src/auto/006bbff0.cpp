#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Base initializer of the visibility controller object (__fastcall: ecx=this, edx unused).
CSF_FUNC(int, __fastcall, GfxVisibilityController_BaseInit, 0x006BEB20, (void* self, void* edx));

// Tail-jumped continuation: runs the base init on the new object, installs the
// derived vtable and returns the object as the result.
static void* __fastcall GfxVisibilityController_InitObject(void* self, void* /*edx*/)
{
    GfxVisibilityController_BaseInit(self, nullptr);
    *(void***)self = CSF_VTBL(0x00862084);
    return self;
}

static void* __cdecl GfxScene_GetVisibilityController()
{
    void* visibilityController = Mem_Alloc(404);
    if (!visibilityController)
        return nullptr;
    return GfxVisibilityController_InitObject(visibilityController, nullptr);
}

HOOK(0x006BBFF0, GfxScene_GetVisibilityController, "auto");

