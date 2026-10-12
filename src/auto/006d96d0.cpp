#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_DestroyStringRanges, 0x00502E00, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, C_00860894_DestroyEmbeddedMember8, 0x006C4A50, (C_00860894* self, void*));
CSF_FUNC(void*, __fastcall, C_00861188_ClearParticleResources, 0x006D8210, (C_00861188* self, void*));

static void __fastcall C_00861188_Destroy(C_00861188* self, void* /*edx*/)
{
    const uint8_t hasParticleResources = CSF_AT(uint8_t, self, 0x99);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008611D4);
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x008611B8);

    if (hasParticleResources)
        C_00861188_ClearParticleResources(self, nullptr);

    C_00860894_DestroyEmbeddedMember8(
        reinterpret_cast<C_00860894*>(
            reinterpret_cast<uint8_t*>(self) + 0x11C),
        nullptr);

    if (void* block = CSF_AT(void*, self, 0x108))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x108) = nullptr;
    }
    if (void* block = CSF_AT(void*, self, 0xF8))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xF8) = nullptr;
    }
    if (void* block = CSF_AT(void*, self, 0xD8))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xD8) = nullptr;
    }
    if (void* block = CSF_AT(void*, self, 0xC8))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xC8) = nullptr;
    }
    if (void* block = CSF_AT(void*, self, 0xB8))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xB8) = nullptr;
    }
    if (void* block = CSF_AT(void*, self, 0xA8))
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xA8) = nullptr;
    }

    SceneLoader_DestroyStringRanges(
        reinterpret_cast<uint8_t*>(self) + 0x8, nullptr);

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861188);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006D96D0, C_00861188_Destroy, "auto");
