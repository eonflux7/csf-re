#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Releases the audio handle stored in the object. Original: int __cdecl sub_7DEF60(std::uint32_t*, int (__cdecl*)(std::uint32_t*, int), int)
CSF_FUNC(int, __cdecl, AudioEffects_ReleaseHandle, 0x7DEF60, (uint32_t* handle, void* callback, int userData));

// Clears the audio handle held at +0x004: calls the object's slot 0xD4 hook first, then releases the handle.
// Returns the release result when a handle was present, otherwise 0.
static int __fastcall C_00863ac0_ClearHandle(C_00863ac0* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xD4 / 4])(self, nullptr);
        result = AudioEffects_ReleaseHandle(reinterpret_cast<uint32_t*>(self->field_0004), nullptr, 0);
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x70F2A0, C_00863ac0_ClearHandle, "auto");
