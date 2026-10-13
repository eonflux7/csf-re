#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static int __fastcall HudSpritesLayout_SetMissionInfoChildText(
    void* self, void* /*edx*/, char* source)
{
    uint8_t resourcePath[256] = {};
    char resourceName[256] = {};

    // These two adjacent pairs also form the final four-word argument.
    alignas(8) uint8_t geometryBytes[0x10];
    void* pair = geometryBytes + 0x8;

    Hud_ExtractResourcePath(source, resourcePath, 256);
    NormalizeResourceName(source, resourceName, 256);

    void* oldChild = CSF_AT(void*, self, 0x158);
    if (oldChild)
    {
        ReleaseObjectReference(static_cast<uint8_t*>(oldChild) + 4, nullptr);
        CSF_AT(void*, self, 0x158) = nullptr;
    }

    void* globals = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globals, nullptr));
    void* resourceManager =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void (__fastcall*)(void*, void*, uint8_t*))
        CSF_AT(void**, resourceManager, 0)[0x10 / 4])(
            resourceManager, nullptr, resourcePath);

    globals = reinterpret_cast<void*>(GetGlobalInt());
    void* controls = MenuControls_ResolveObject(globals, nullptr);
    int result =
        ((int (__fastcall*)(void*, void*, char*))
            CSF_AT(void**, controls, 0)[0x38 / 4])(
                controls, nullptr, resourceName);
    CSF_AT(uint32_t, self, 0x158) = static_cast<uint32_t>(result);

    if (!result)
        return result;

    CSF_AT(uint32_t, pair, 0x4) = CSF_AT(uint32_t, self, 0x138);
    CSF_AT(uint32_t, pair, 0x0) = CSF_AT(uint32_t, self, 0x13C);
    void* child = reinterpret_cast<void*>(result);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr, pair);

    CSF_AT(uint32_t, pair, 0x4) = CSF_AT(uint32_t, self, 0x144);
    CSF_AT(uint32_t, pair, 0x0) = CSF_AT(uint32_t, self, 0x140);
    child = CSF_AT(void*, self, 0x158);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, child, 0)[0x38 / 4])(child, nullptr, pair);

    child = CSF_AT(void*, self, 0x158);
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, child, 0)[0x44 / 4])(child, nullptr, 0);

    CSF_AT(uint32_t, geometryBytes, 0x0) = CSF_AT(uint32_t, self, 0x14C);
    CSF_AT(uint32_t, geometryBytes, 0x4) = CSF_AT(uint32_t, self, 0x148);
    CSF_AT(uint32_t, pair, 0x4) = CSF_AT(uint32_t, self, 0x150);
    CSF_AT(uint32_t, pair, 0x0) = CSF_AT(uint32_t, self, 0x154);
    child = CSF_AT(void*, self, 0x158);
    return ((int (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, child, 0)[0x4C / 4])(
            child, nullptr, geometryBytes);
}

HOOK(0x004370E0, HudSpritesLayout_SetMissionInfoChildText, "auto");
