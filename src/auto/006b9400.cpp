#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __cdecl, GfxObjectProps_WriteMaterial, 0x006B90B0, (uint32_t* a1, int a2, int a3, int a4, int a5));

CSF_GLOBAL(int32_t, g_ObjectPropsStreamVersion, 0x008A7E1C);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

static uint32_t* __cdecl GfxObjectProps_WriteUserData(
    uint32_t* stream, int /*unused*/, int entryOffset)
{
    void* properties = CSF_AT(
        void*, static_cast<uint8_t*>(g_EntryTable) + entryOffset, 0x04);

    int32_t header = g_ObjectPropsStreamVersion;
    RwStreamWriteInt32(stream, &header, 4);

    header = properties != nullptr;
    RwStreamWriteInt32(stream, &header, 4);

    if (properties)
    {
        auto* serialized = static_cast<uint32_t*>(Mem_Alloc(20));
        serialized[0] = CSF_AT(uint8_t, properties, 0x01) != 0;
        serialized[1] = CSF_AT(uint8_t, properties, 0x00) != 0;
        serialized[2] = CSF_AT(uint32_t, properties, 0x0C);
        serialized[3] = CSF_AT(uint32_t, properties, 0x08);
        serialized[4] = CSF_AT(uint8_t, properties, 0x11);

        RwStreamWriteInt32(stream, serialized, 20);
        Mem_Free(serialized);

        GfxObjectProps_WriteMaterial(
            stream, CSF_AT(int, properties, 0x04), 0, 0, 0);
    }

    return stream;
}

HOOK(0x006B9400, GfxObjectProps_WriteUserData, "auto");
