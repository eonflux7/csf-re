#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, InitializeResourceWrapper, 0x00737A70,
         (void* self, void* edx, C_008655e8* owner));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

static int __fastcall C_008655e8_CreateTextureWrapper(
    C_008655e8* self, void* /*edx*/, int textureKey)
{
    const int texture =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x7C / 4])(
                self, nullptr, textureKey);

    if (!texture)
        return 0;

    void* memory = Mem_Alloc(0x90);
    void* wrapper = nullptr;
    if (memory)
        wrapper = InitializeResourceWrapper(memory, nullptr, self);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, wrapper, 0)[0x1C / 4])(
            wrapper, nullptr, texture);

    RwTextureDestroy(texture);
    return reinterpret_cast<int>(wrapper);
}

HOOK(0x007267D0, C_008655e8_CreateTextureWrapper, "auto");
