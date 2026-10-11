#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int, __fastcall, GfxObjectProps_ReservePropertyStorage,
         0x006B9EE0, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static void* GfxObjectProps_CreatePropertyStorage()
{
    void* storage = Mem_Alloc(16);
    if (storage)
    {
        CSF_AT(std::uint32_t, storage, 0x04) = 0;
        CSF_AT(std::uint32_t, storage, 0x08) = 0;
        CSF_AT(std::uint32_t, storage, 0x0C) = 0;
        GfxObjectProps_ReservePropertyStorage(storage, nullptr, 200);
    }
    return storage;
}

static int __cdecl GfxObjectProps_InitializeEnginePropertyStorage(
    int object, int pluginOffset)
{
    auto* properties =
        static_cast<std::uint8_t*>(g_pRwEngineInstance) + pluginOffset;

    CSF_AT(void*, properties, 0x00) = GfxObjectProps_CreatePropertyStorage();
    CSF_AT(void*, properties, 0x04) = GfxObjectProps_CreatePropertyStorage();
    CSF_AT(void*, properties, 0x08) = GfxObjectProps_CreatePropertyStorage();

    return object;
}

HOOK(0x006BAF80, GfxObjectProps_InitializeEnginePropertyStorage, "auto");
