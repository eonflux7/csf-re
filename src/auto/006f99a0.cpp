#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureRead, 0x006319C0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00862b04_UpdateFilmCoordinates, 0x006F98F0, (C_00862b04* self, void*));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));

static bool __fastcall C_00862d48_InitializeNoiseEffect(
    C_00862d48* self, void* /*edx*/, int initializationArgument)
{
    if (self->field_0008 && self->field_0060)
        self->field_0060 = 0;

    ScreenEffects_UpdateBase(self, nullptr);
    InitializeScreenEffect(self, nullptr, initializationArgument);

    CSF_AT(uint8_t, self, 0x30) = 0;
    self->field_0060 = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;

    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr);

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

    global = reinterpret_cast<void*>(GetGlobalInt());
    accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* textureManager = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

    ((void (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, textureManager, 0)[0x10 / 4])(
            textureManager, nullptr, CSF_STR(0x00862AB8, "Gfx/Textures/"));

    int texture = RwTextureRead(
        reinterpret_cast<int>(CSF_STR(0x00862AC8, "Noise")), 0);
    CSF_AT(uint32_t, self, 0x64) = texture;
    bool loaded = texture != 0;

    if (loaded)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) = 4;
        self->field_0060 = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x64)),
            0);
    }

    C_00862b04_UpdateFilmCoordinates(
        reinterpret_cast<C_00862b04*>(self), nullptr);
    self->field_0008 = 1;

    if (!loaded)
    {
        if (self->field_0060)
            self->field_0060 = 0;
        ScreenEffects_UpdateBase(self, nullptr);
    }

    return loaded;
}

HOOK(0x006F99A0, C_00862d48_InitializeNoiseEffect, "auto");
