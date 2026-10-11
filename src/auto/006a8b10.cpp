#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

static std::uint8_t* __fastcall C_0085fe28_CreateGatheringRecord(
    C_0085fe28* self, void* /*edx*/, int identifier, void* gatheringHandle)
{
    ((void(__fastcall*)(C_0085fe28*, void*))self->vtbl[0x24 / 4])(
        self, nullptr);

    auto* source = CSF_AT(std::uint8_t*, gatheringHandle, 0x04);
    auto* record = static_cast<std::uint8_t*>(Mem_Alloc(696));
    if (!record)
        return record;

    CSF_AT(std::uint8_t, record, 0x00) = 0xFF;
    CSF_AT(std::uint8_t, record, 0x01) = 0;
    CSF_AT(std::uint8_t, record, 0x02) = 0;
    CSF_AT(int, record, 0x2B4) = identifier;
    CSF_AT(std::uint8_t, record, 0x83) = 0;
    CSF_AT(std::uint8_t, record, 0xA3) = 0;
    CSF_AT(int, record, 0x2A4) = 0;
    CSF_AT(int, record, 0x2A8) = 0;
    CSF_AT(int, record, 0x2AC) = -1;
    CSF_AT(int, record, 0x2B0) = 0;

    CSF_AT(std::uint8_t, record, 0x00) =
        CSF_AT(std::uint8_t, source, 0x30);
    CSF_AT(std::uint8_t, record, 0x01) =
        CSF_AT(std::uint8_t, source, 0x32);
    CSF_AT(std::uint8_t, record, 0x02) =
        CSF_AT(std::uint8_t, source, 0x33);

    _strncpy(reinterpret_cast<char*>(record + 0x83),
             reinterpret_cast<const char*>(source + 0x34), 0x20u);
    _strncpy(reinterpret_cast<char*>(record + 0x03),
             CSF_AT(const char*, source, 0x24), 0x7Fu);
    _strncpy(reinterpret_cast<char*>(record + 0xA3),
             reinterpret_cast<const char*>(source + 0x54), 0x1FFu);

    CSF_AT(int, record, 0x2A4) = -1;
    CSF_AT(int, record, 0x2A8) = CSF_AT(std::int8_t, source, 0x12);
    CSF_AT(std::uint32_t, record, 0x2B0) =
        CSF_AT(std::uint32_t, source, 0x254);

    return record;
}

HOOK(0x006A8B10, C_0085fe28_CreateGatheringRecord, "auto");
