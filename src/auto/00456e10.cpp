#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_008312e0_ResetPortraitState, 0x00456CE0, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static int __fastcall C_008312e0_ReleasePortraits(C_008312e0* self, void* /*edx*/)
{
    Hud_UnregisterObject(reinterpret_cast<int>(self));
    C_008312e0_ResetPortraitState(self, nullptr);

    for (uint32_t index = 0; index < self->field_0018; ++index)
    {
        void* portrait = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0010)[index]);
        if (portrait)
        {
            void* controls = MenuControls_ResolveObject(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            uint32_t control = CSF_AT(uint32_t, portrait, 0x50);
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, controls, 0)[0x88 / 4])(
                    controls, nullptr, control);

            controls = MenuControls_ResolveObject(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            control = CSF_AT(uint32_t, portrait, 0x54);
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, controls, 0)[0x88 / 4])(
                    controls, nullptr, control);

            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, portrait, 0)[0])(
                    portrait, nullptr, 1);
        }
    }

    void* portraits = reinterpret_cast<void*>(self->field_0010);
    self->field_0014 = 0;
    self->field_0018 = 0;
    if (portraits)
    {
        Mem_Free(portraits);
        self->field_0010 = 0;
    }

    int result = static_cast<int>(self->field_0020);
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(result));
        self->field_0020 = 0;
    }
    return result;
}

HOOK(0x00456E10, C_008312e0_ReleasePortraits, "auto");
