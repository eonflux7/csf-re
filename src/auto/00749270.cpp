#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, SpawnPoints_CopyRecord, 0x005557C0, (char* self, void* , char* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(char*, __fastcall, CopyRecordFromSource_749060, 0x00749060, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ListContainer_RemoveNode, 0x007490E0, (uint8_t* self, void* , uint32_t* outNext, uint8_t* node));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char* __fastcall SpawnPoints_PopFrontRecord(void* self, void* /*edx*/, char* destination)
{
    alignas(8) uint8_t recordBytes[0x44];

    CSF_AT(uint32_t, recordBytes, 0x20) = 7;
    CSF_AT(int, recordBytes, 0x1C) = 0;
    CSF_AT(uint16_t, recordBytes, 0x0C) = 0;
    CSF_AT(uint8_t, recordBytes, 0x28) = 0;
    TextInputSubobject_Init(
        reinterpret_cast<S_004f21d0*>(recordBytes + 0x08),
        nullptr, &g_byte_82E65C);

    CSF_AT(int, recordBytes, 0x30) = -1;
    CSF_AT(int, recordBytes, 0x34) = -1;
    CSF_AT(int, recordBytes, 0x00) = -1;
    CSF_AT(int, recordBytes, 0x04) = -1;
    CSF_AT(int, recordBytes, 0x38) = -1;

    const bool hasRecord = CSF_AT(uint32_t, self, 0x464) != 0;
    CSF_AT(int, recordBytes, 0x24) = 0;
    CSF_AT(int, recordBytes, 0x2C) = 0;
    CSF_AT(uint8_t, recordBytes, 0x3E) = 0;
    CSF_AT(uint8_t, recordBytes, 0x3D) = 0;
    CSF_AT(uint8_t, recordBytes, 0x3C) = 0;

    if (hasRecord)
    {
        SpawnPoints_CopyRecord(
            reinterpret_cast<char*>(recordBytes), nullptr,
            reinterpret_cast<char*>(
                CSF_AT(uint8_t*, CSF_AT(void*, self, 0x460), 0) + 8));

        ListContainer_RemoveNode(
            reinterpret_cast<uint8_t*>(self) + 0x45C, nullptr,
            reinterpret_cast<uint32_t*>(recordBytes + 0x40),
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x460), 0));
    }

    CopyRecordFromSource_749060(
        destination, nullptr, reinterpret_cast<int>(recordBytes));

    if (CSF_AT(uint32_t, recordBytes, 0x20) >= 8)
    {
        void* block = CSF_AT(void*, recordBytes, 0x0C);
        if (block)
            Mem_FreeDebug(block);
    }

    return destination;
}

HOOK(0x00749270, SpawnPoints_PopFrontRecord, "auto");
