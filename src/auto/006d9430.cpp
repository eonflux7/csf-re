#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(std::uint32_t, __fastcall, S_006d8f80_UpdateParticlePositions, 0x006D8F80, (S_006d8f80* self, void* , int* source));

// Per-frame update of a PTank skin object: when its active flag is set and its frame counter is zero,
// refreshes particle positions from the global frame object, notifies each entry for the current
// frame counter, then advances the frame counter.
static std::uint32_t __fastcall GfxSkin_RenderGeometry(S_006d8f80* self, void* /*edx*/)
{
    std::uint32_t result = self->field_00a4;
    if (result != 0)
    {
        result = self->field_00f4;
        if (result == 0)
        {
            int globalValue = GetGlobalValue_6C5760();
            void* globalObject = reinterpret_cast<void*>(static_cast<std::intptr_t>(globalValue));
            int* frameSource = CSF_AT(int*, globalObject, 0xF0);

            // Reference to the S_006d8f80 vtable (no behavior).
            (void)CSF_VTBL(0x006D8F80);
            S_006d8f80_UpdateParticlePositions(self, nullptr, frameSource);

            std::uint32_t i = 0;
            result = self->field_00e0;
            while (i < result)
            {
                auto* entries = reinterpret_cast<std::uint32_t*>(self->field_00d8);
                void* entry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(entries[i]));
                void* subObject = static_cast<std::uint8_t*>(entry) + 0x20;

                // Slot 6 (offset 0x18) of the sub-object's vtable, called with the current frame counter.
                void* found = ((void*(__fastcall*)(void*, void*, std::uint32_t))CSF_AT(void**, subObject, 0)[6])(
                    subObject, nullptr, self->field_00f4);

                if (found != nullptr)
                {
                    // Slot 31 (offset 0x7C) of the returned object's vtable, no arguments.
                    ((void(__fastcall*)(void*, void*))CSF_AT(void**, found, 0)[31])(found, nullptr);
                }

                ++i;
                result = self->field_00e0;
            }

            self->field_00f4++;
        }
    }
    return result;
}

HOOK(0x006D9430, GfxSkin_RenderGeometry, "auto");
