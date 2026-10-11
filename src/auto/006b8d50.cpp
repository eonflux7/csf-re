#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, sub_6B8CC0, 0x006B8CC0, (int a1));

CSF_GLOBAL(uint32_t, g_ObjectPropsTableBase, 0x008BB488);
CSF_GLOBAL(int, g_PropertyIndexMin, 0x008A7E3C);
CSF_GLOBAL(int, g_PropertyIndexMax, 0x008A7E28);

// Reads one property block from the stream into the buffer owned by the table entry.
// Returns 0 when the stored index is out of range; otherwise returns the stream.
static int __cdecl GfxObjectProps_ReadPropertyBlock(int stream, int /*unused*/, int offset)
{
    uint8_t* tableBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_ObjectPropsTableBase));

    // Free any previously loaded buffer held by this entry.
    uint32_t* entry = *reinterpret_cast<uint32_t**>(tableBase + offset + 4);
    if (entry[0] != 0) {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(entry[0])));
        entry[0] = 0;
    }

    uint32_t* streamPtr = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(stream));

    int readValue = 0;
    RwStreamReadInt32(streamPtr, &readValue, 4);
    if (readValue < g_PropertyIndexMin || readValue > g_PropertyIndexMax) {
        return 0;
    }

    RwStreamReadInt32(streamPtr, &readValue, 4);
    if (readValue != 0) {
        uint16_t byteCount = *reinterpret_cast<uint16_t*>(static_cast<uintptr_t>(offset) + 0x84);
        void* buffer = sub_6B8CC0(offset);
        RwStreamRead(streamPtr, buffer, byteCount);
    }

    return stream;
}

HOOK(0x6B8D50, GfxObjectProps_ReadPropertyBlock, "auto");
