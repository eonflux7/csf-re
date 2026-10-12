#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, ResourceMatchesName, 0x006DD340, (void* self, void* edx, int a4));
CSF_FUNC(int*, __fastcall, CreateParticleResourceBlock, 0x006DE210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindCachedParticleResource, 0x006E0FD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, RegisterParticleResource, 0x006E13B0, (void* self, void* edx, void* a2, int a3));

CSF_GLOBAL(S_006dcf90, g_DefaultParticleResource, 0x008BB740);

static int __fastcall GfxParticleEmitters_ResolveParticleResource(void* self, void* /*edx*/, C_0082d410* a2)
{
    // Resource cache embedded in the emitter at +0x50
    void* resourceCache = static_cast<uint8_t*>(self) + 0x50;

    int cachedResult = FindCachedParticleResource(resourceCache, nullptr, reinterpret_cast<uint32_t*>(a2));
    if (cachedResult != 0)
        return cachedResult;

    void* rawBlock = Mem_Alloc(0x154);
    S_006dcf90* block = nullptr;
    if (rawBlock != nullptr)
        block = reinterpret_cast<S_006dcf90*>(CreateParticleResourceBlock(rawBlock, nullptr));

    uint32_t* nameSource;
    if (CSF_AT(uint32_t, a2, 0x18) < 0x10u)
        nameSource = &a2->field_0004;
    else
        nameSource = reinterpret_cast<uint32_t*>(a2->field_0004);

    if (ResourceMatchesName(block, nullptr, reinterpret_cast<int>(nameSource)))
    {
        RegisterParticleResource(resourceCache, nullptr, a2, reinterpret_cast<int>(block));
    }
    else
    {
        std::memcpy(block, &g_DefaultParticleResource, sizeof(S_006dcf90));
        RegisterParticleResource(resourceCache, nullptr, a2, reinterpret_cast<int>(block));
    }
    return reinterpret_cast<int>(block);
}

HOOK(0x006E15F0, GfxParticleEmitters_ResolveParticleResource, "auto");
