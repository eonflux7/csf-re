#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, ZlibLog_InitWithDefaultParams, 0x006A1840, (uint32_t* a1, int a2, uint8_t* a3, int a4));
CSF_FUNC(int, __cdecl, DlgLog_AppendMessage, 0x006A1A10, (uint32_t* logWindow, uint8_t* message, int messageLength));

static uint32_t* __fastcall C_0085e748_ConstructCompressionStream(
    C_0085e748* self, void* /*edx*/, int contextValue, char decompress)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E748);
    self->field_0004 = contextValue;
    self->field_000c = CoreMemory_AllocUntagged(0x4000);
    self->field_0008 = decompress;

    void* stream = reinterpret_cast<void*>(CoreMemory_AllocUntagged(56));
    self->field_0010 = reinterpret_cast<std::uintptr_t>(stream);
    std::memset(stream, 0, 0x38);

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x20) =
        (uint32_t)CSF_ADDR_FN(0x00699DA0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x24) =
        (uint32_t)CSF_ADDR_FN(0x00699DC0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x2C) = 0;

    if (decompress)
    {
        DlgLog_AppendMessage(
            reinterpret_cast<uint32_t*>(self->field_0010),
            reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x0085E758, "1.1.4"))),
            56);
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x00) = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x04) = 0;
    }
    else
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10) = 0x4000;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x0C) =
            self->field_000c;
        ZlibLog_InitWithDefaultParams(
            reinterpret_cast<uint32_t*>(self->field_0010),
            7,
            reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x0085E758, "1.1.4"))),
            56);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069A080, C_0085e748_ConstructCompressionStream, "auto");
