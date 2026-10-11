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

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Releases the previously resolved object state (called when a resolved object already exists).
CSF_FUNC(int, __fastcall, SceneLoader_ReleaseOwnedObject, 0x0049FFC0, (void* self, void* edx));

// Finishes setup after the font has been resolved.
CSF_FUNC(int, __fastcall, SceneLoader_FinishSetup, 0x004A0580, (void* self, void* edx));

static char __fastcall SceneLoader_SetResolvedObject(C_008343a4* self, void* /*edx*/, uint32_t resolvedObject)
{
    // Drop any previously resolved state first.
    if (self->field_0004)
    {
        SceneLoader_ReleaseOwnedObject(self, nullptr);
        self->field_0004 = 0;
    }

    CSF_AT(uint8_t, self, 0x05) = 1;
    self->field_0008 = 0;
    self->field_000c = resolvedObject;

    // Allocate a 4-byte cell holding a back-pointer to this object.
    void* cell = Mem_Alloc(4);
    if (cell)
        *static_cast<void**>(cell) = nullptr;
    else
        cell = nullptr;

    self->field_0030 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cell));
    *static_cast<C_008343a4**>(cell) = self;

    // Ask the resolved object (vtable slot 0x4C / 4 = 19) for the font handle.
    void* fontProvider = reinterpret_cast<void*>(self->field_000c);
    using GetFontFn = uint32_t(__fastcall*)(void* self, void* edx, const char* name);
    GetFontFn getFont = reinterpret_cast<GetFontFn>(CSF_AT(void**, fontProvider, 0)[0x4C / 4]);
    self->field_0008 = getFont(fontProvider, nullptr, CSF_STR(0x008343B0, "menus\\Ingame_01.fnt"));

    SceneLoader_FinishSetup(self, nullptr);
    self->field_0004 = 1;
    return 1;
}

HOOK(0x004A0880, SceneLoader_SetResolvedObject, "auto");
