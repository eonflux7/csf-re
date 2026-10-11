#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, ZlibInflateInit, 0x006A1A10, (uint32_t* a1, uint8_t* a2, int a3));
CSF_FUNC(void*, __fastcall, GrowRecordArray, 0x006D9EB0, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_RecordCount, 0x008A6BB0);
CSF_GLOBAL(uint8_t, g_RecordArray, 0x008A6BAC);

// Allocates a 56-byte zlib z_stream, wires its zalloc/zfree/opaque,
// and calls inflateInit_(strm, "1.1.4", sizeof(z_stream)).
static void** __fastcall CreateInflateStream(S_006a30a0* self, void* /*edx*/) {
    self->field_0000 = reinterpret_cast<uint32_t>(Mem_Alloc(56));

    const uint32_t count = g_RecordCount;
    if (count < 10u) {
        GrowRecordArray(static_cast<void*>(&g_RecordArray), nullptr,
                        static_cast<int>(10u - count));
    }

    uint8_t* stream = reinterpret_cast<uint8_t*>(self->field_0000);
    std::memset(stream, 0, 0x38);

    CSF_AT(uint32_t, stream, 0x20) = reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006A2F90)); // zalloc
    CSF_AT(uint32_t, stream, 0x24) = reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006A2EB0)); // zfree
    CSF_AT(uint32_t, stream, 0x2C) = 0;                                                    // opaque

    ZlibInflateInit(reinterpret_cast<uint32_t*>(self->field_0000),
                    reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x0085E758, "1.1.4"))),
                    56);

    stream = reinterpret_cast<uint8_t*>(self->field_0000);
    CSF_AT(uint32_t, stream, 0x00) = 0; // next_in
    CSF_AT(uint32_t, stream, 0x04) = 0; // avail_in

    return reinterpret_cast<void**>(self);
}

HOOK(0x006A30A0, CreateInflateStream, "auto");
