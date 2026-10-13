#include <cstddef>
#include "globals.h"
#include "hook.h"

// Engine lookups reached through the global interface (bound by address).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

// Registers the environment texture paths with the object returned by the resolved accessor.
// Original: const char *sub_6BC0F0(), ret 0. Returns the list terminator (nullptr).
static const char* RegisterEnvironmentTexturePaths()
{
    const char* envTexturePath = CSF_STR(0x008605B8, "gfx\\textures\\env.png");
    const char* noiseTexturePath = CSF_STR(0x008605A0, "gfx\\textures\\Noise.png");

    // Stack list of texture paths, terminated by nullptr (same order as the original frame).
    const char* texturePaths[3] = {
        envTexturePath,
        noiseTexturePath,
        nullptr,
    };

    // Resolve the accessor object from the global interface.
    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);

    // vtbl slot 4 (offset 0x10): returns the texture registry object.
    int registry = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(accessor), 0)[4])(
        reinterpret_cast<void*>(accessor), nullptr);

    // Add each path to the registry (vtbl slot 7, offset 0x1C), until the null terminator.
    const char* result = texturePaths[0];
    std::size_t index = 0;
    do {
        ((void(__fastcall*)(void*, void*, const char*))CSF_AT(void**, reinterpret_cast<void*>(registry), 0)[7])(
            reinterpret_cast<void*>(registry), nullptr, result);
        ++index;
        result = texturePaths[index];
    } while (result);

    return result;
}

HOOK(0x006BC0F0, RegisterEnvironmentTexturePaths, "auto");
