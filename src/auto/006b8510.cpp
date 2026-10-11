#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamWrite, 0x00629530, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));

CSF_GLOBAL(uint32_t, g_ObjPropsBaseOffset, 0x008BB488);
CSF_GLOBAL(uint32_t, g_StreamRecordTag, 0x008A7E28);

// Writes one user-data property record (tag, has-data flag, optional payload) to an RW stream.
static int __cdecl GfxObjectProps_WriteStreamRecord(int a1, int a2, int a3) {
    auto* stream = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
    auto* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3));

    uint16_t payloadLength = CSF_AT(uint16_t, obj, 0x84);

    // Pointer chain: slot = *(base + obj + 4); payloadPtr = *slot.
    uint32_t slotValue = *reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(g_ObjPropsBaseOffset) + static_cast<uintptr_t>(a3) + 4);
    uint32_t payloadPtr = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(slotValue));

    uint32_t tag = g_StreamRecordTag;
    RwStreamWriteInt32(stream, &tag, 4);

    uint32_t hasPayload = (payloadPtr != 0) ? 1u : 0u;
    RwStreamWriteInt32(stream, &hasPayload, 4);

    if (payloadPtr != 0) {
        RwStreamWrite(stream,
                      reinterpret_cast<void*>(static_cast<uintptr_t>(payloadPtr)),
                      static_cast<unsigned int>(payloadLength));
    }
    return a1;
}

HOOK(0x006B8510, GfxObjectProps_WriteStreamRecord, "auto");
