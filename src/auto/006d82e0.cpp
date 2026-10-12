#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(std::uint32_t, __fastcall, S_006d7c80_UpdateParticlePositions, 0x006D7C80, (S_006d7c80* self, void* , void* source));

// Per-frame update of a PTank skin object: once per frame (while its counter is zero),
// refreshes particle positions from the global frame value, then notifies each entry.
static std::uint32_t __fastcall GfxSkin_PrepareSkinObject(S_006d7c80* self, void* /*edx*/)
{
    if (self->field_00a4 == 0)
        return 0;
    if (self->field_00ec != 0)
        return self->field_00ec;

    int globalValue = GetGlobalValue_6C5760();
    void* globalObject = reinterpret_cast<void*>(static_cast<std::intptr_t>(globalValue));
    std::uint32_t frameValue = CSF_AT(std::uint32_t, globalObject, 0xF0);

    // Reference to the S_006d7c80 vtable slot used by the original call site (no behavior).
    (void)CSF_VTBL(0x006D7C80);
    S_006d7c80_UpdateParticlePositions(self, nullptr, reinterpret_cast<void*>(static_cast<std::uintptr_t>(frameValue)));

    for (std::uint32_t i = 0; i < self->field_00c0; ++i)
    {
        auto* entries = reinterpret_cast<std::uint32_t*>(self->field_00b8);
        void* entry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(entries[i]));
        void* subObject = static_cast<std::uint8_t*>(entry) + 0x20;

        // Slot 6 (offset 0x18) of the sub-object's vtable, called with the current frame counter.
        void* found = ((void*(__fastcall*)(void*, void*, std::uint32_t))CSF_AT(void**, subObject, 0)[6])(
            subObject, nullptr, self->field_00ec);

        if (found != nullptr)
        {
            // Slot 31 (offset 0x7C) of the returned object's vtable, no arguments.
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, found, 0)[31])(found, nullptr);
        }
    }

    self->field_00ec++;
    return self->field_00c0;
}

HOOK(0x006D82E0, GfxSkin_PrepareSkinObject, "auto");
