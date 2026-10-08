#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, AllocateMemory, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, InitializeTextureView, 0x00739110,
         (void* self, void* edx, C_008655e8* owner));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

// Allocates a 0x48-byte view object, initializes it with the owner, then
// resolves a texture from the owner and hands it to the new view.
static int __fastcall C_008655e8_CreateTextureViewEx(
    C_008655e8* self, void* /*edx*/, int textureKey)
{
    void* memory = AllocateMemory(0x48);
    void* view = nullptr;
    if (memory)
        view = InitializeTextureView(memory, nullptr, self);

    if (textureKey)
    {
        const int texture =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x7C / 4])(
                    self, nullptr, textureKey);

        if (texture)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, view, 0)[0x24 / 4])(
                    view, nullptr, texture);

            RwTextureDestroy(texture);
        }
    }

    return reinterpret_cast<int>(view);
}

HOOK(0x007269F0, C_008655e8_CreateTextureViewEx, "auto");
