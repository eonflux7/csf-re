#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00861bd8_InitWithSkinFlags, 0x006E5210, (C_00861bd8* self, void* , int** a2, char a3));

// Returns the cached skin-flags object owned by this object, creating it on first use.
static void* __fastcall C_00862640_GetOrCreateSkinFlagsObject(C_00862640* self, void* /*edx*/)
{
    if (!self->field_0048)
    {
        // Ask the inner object (at field_001c) for its skin-flags table via vtable slot 0x1C.
        void* inner = reinterpret_cast<void*>(self->field_001c);
        int** skinFlags = ((int**(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x1C / 4])(inner, nullptr);

        C_00861bd8* binding = reinterpret_cast<C_00861bd8*>(CoreMemory_AllocUntagged(20));
        if (binding)
        {
            void* result = C_00861bd8_InitWithSkinFlags(binding, nullptr, skinFlags, 0);
            self->field_0048 = reinterpret_cast<uintptr_t>(result);
            return result;
        }
        self->field_0048 = 0;
    }
    return reinterpret_cast<void*>(self->field_0048);
}

HOOK(0x006F2D50, C_00862640_GetOrCreateSkinFlagsObject, "auto");
