#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureRead, 0x006319C0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static bool __fastcall C_00862c74_LoadOldFilmResources(
    C_00862c74* self, void* /*edx*/)
{
    char filename[0x100];
    alignas(8) uint8_t managerRequest[0xC];
    CSF_AT(uint32_t, managerRequest, 0x8) =
        reinterpret_cast<uint32_t>(self);

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

    accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* texturePaths = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));
    ((void(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, texturePaths, 0)[0x10 / 4])(
            texturePaths, nullptr, CSF_STR(0x00862BD4, "Gfx/Textures/"));

    uint32_t texture = static_cast<uint32_t>(RwTextureRead(
        reinterpret_cast<int>(CSF_STR(0x00862B9C, "OldFilmDirty")), 0));
    self->field_0060 = texture;
    if (!texture)
        return false;
    CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) = 4;

    texture = static_cast<uint32_t>(RwTextureRead(
        reinterpret_cast<int>(CSF_STR(0x00862BAC, "OldFilmDirty2")), 0));
    self->field_0064 = texture;
    if (!texture)
        return false;
    CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) = 4;

    texture = static_cast<uint32_t>(RwTextureRead(
        reinterpret_cast<int>(CSF_STR(0x00862BBC, "OldFilmLine")), 0));
    self->field_0068 = texture;
    if (!texture)
        return false;
    CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) = 4;

    uint8_t* hairSlot = reinterpret_cast<uint8_t*>(self) + 0x6C;
    bool loaded = true;
    int hairIndex = 0;
    while (loaded)
    {
        ++hairIndex;
        _sprintf(filename, CSF_STR(0x00862C24, "%s%i.png"),
            CSF_STR(0x00862BC8, "OldFilmHair"), hairIndex);
        texture = static_cast<uint32_t>(
            RwTextureRead(reinterpret_cast<int>(filename), 0));
        loaded = texture != 0;
        CSF_AT(uint32_t, hairSlot, 0) = texture;
        if (texture)
            CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) = 4;
        hairSlot += 4;

        if (hairIndex >= 4)
        {
            if (loaded)
            {
                CSF_AT(uint32_t, managerRequest, 0x4) = 0;
                CSF_AT(uint32_t, managerRequest, 0) = 4;
                void* manager = reinterpret_cast<void*>(
                    ResolveRwManager(GetGlobalInt(), nullptr));
                int effect = ((int(__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, manager, 0)[0x20 / 4])(
                        manager, nullptr,
                        reinterpret_cast<uint32_t*>(managerRequest));

                reinterpret_cast<C_00862c74*>(
                    CSF_AT(uint32_t, managerRequest, 0x8))->field_0080 =
                        static_cast<uint32_t>(effect);

                void* effectObject = reinterpret_cast<void*>(effect);
                ((void(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, effectObject, 0)[0x1C / 4])(
                        effectObject, nullptr, 0, 10);
            }
            return loaded;
        }
    }
    return loaded;
}

HOOK(0x006F9E60, C_00862c74_LoadOldFilmResources, "auto");
