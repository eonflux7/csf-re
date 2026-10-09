#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270,
         (const void* path));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390,
         (void* self, void* edx));

// Hex-Rays' a1@<esi> is the `push esi` callee-save around the slot 0x38 call (paired with the final pop esi): the
// slot takes only the name, and this function only its one stack argument (callers clean 4 bytes).
static void* __cdecl MenuMpPause_LoadTexture(const void* textureName)
{
    RwImageSetPath(CSF_STR(0x00841C64, "Menus/Textures/"));

    const int globalContext = GetGlobalInt();
    void* provider = MenuControls_ResolveObject(
        reinterpret_cast<void*>(globalContext), nullptr);

    const char* name;
    if (CSF_AT(std::uint32_t, textureName, 0x18) < 0x10u)
        name = reinterpret_cast<const char*>(textureName) + 4;
    else
        name = CSF_AT(const char*, textureName, 0x04);

    using LoadTexture = void* (__fastcall*)(void*, void*, const char*);

    void* texture =
        reinterpret_cast<LoadTexture>(CSF_AT(void**, provider, 0)[0x38 / 4])(
            provider, nullptr, name);

    if (texture)
    {
        using SetTextureRegion = void (__fastcall*)(
            void*, void*, const std::uint32_t*);

        auto setRegion = reinterpret_cast<SetTextureRegion>(
            CSF_AT(void**, texture, 0)[0x4C / 4]);

        std::uint32_t region[4] = {
            0, 0, 0x3F800000u, 0x3F800000u
        };
        setRegion(texture, nullptr, region);
    }

    return texture;
}

HOOK(0x00533D90, MenuMpPause_LoadTexture, "auto");
