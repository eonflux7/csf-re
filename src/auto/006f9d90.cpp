#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));

static char __fastcall C_00862c74_ReleaseTexturesAndUpdate(
    C_00862c74* self, void* /*edx*/)
{
    if (self->field_0008)
    {
        if (self->field_0080)
        {
            void* manager = reinterpret_cast<void*>(
                ResolveRwManager(GetGlobalInt(), nullptr));
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[0x24 / 4])(
                    manager, nullptr, self->field_0080);
            self->field_0080 = 0;
        }

        if (self->field_007c)
        {
            void* manager = reinterpret_cast<void*>(
                ResolveRwManager(GetGlobalInt(), nullptr));
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[0x24 / 4])(
                    manager, nullptr, self->field_007c);
            self->field_007c = 0;
        }

        if (self->field_0060)
        {
            RwTextureDestroy(static_cast<int>(self->field_0060));
            self->field_0060 = 0;
        }

        if (self->field_0064)
        {
            RwTextureDestroy(static_cast<int>(self->field_0064));
            self->field_0064 = 0;
        }

        uint8_t* textureSlot = reinterpret_cast<uint8_t*>(self) + 0x6C;
        int remaining = 4;
        do
        {
            if (CSF_AT(uint32_t, textureSlot, 0))
            {
                RwTextureDestroy(
                    static_cast<int>(CSF_AT(uint32_t, textureSlot, 0)));
                CSF_AT(uint32_t, textureSlot, 0) = 0;
            }
            textureSlot += 4;
            --remaining;
        } while (remaining);

        if (self->field_0068)
        {
            RwTextureDestroy(static_cast<int>(self->field_0068));
            self->field_0068 = 0;
        }
    }

    return ScreenEffects_UpdateBase(self, nullptr);
}

HOOK(0x006F9D90, C_00862c74_ReleaseTexturesAndUpdate, "auto");
