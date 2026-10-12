#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));
CSF_FUNC(void, __fastcall, NetPlayerDdl_ProcessBaseChunks, 0x007A39C0, (void* self, void* edx, void* a2));

static void C_008622e8_ProcessPropertyChunk(
    C_008622e8* self, C_008622e8* other, uint8_t propertyId,
    int memberOffset, void** property, uint8_t* propertyKey)
{
    const int objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    *propertyKey = propertyId;
    const int objectAddress = LookupObjectByIndex(objectIndex);
    auto* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(objectAddress));

    NetCogibleDdl_FindProperty(object + 0x0C, nullptr, property, propertyKey);
    void* processor = CSF_AT(void*, *property, 0x10);
    ProcessDDLChunk(
        processor, nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + memberOffset),
        propertyId, other);
}

static void __fastcall C_008622e8_ProcessPlayerDdlChunks(
    C_008622e8* self, void* /*edx*/, C_008622e8* other)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    alignas(8) uint8_t remainingKeyBytes[0x4];
    auto** property = reinterpret_cast<void**>(propertyBytes);

    C_008622e8_ProcessPropertyChunk(self, other, 4, 1088, property, firstKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 5, 1091, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 6, 1094, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 7, 1100, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 8, 1103, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 9, 1106, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 10, 1112, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 11, 1640, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 12, 1652, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 13, 1664, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 14, 1670, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 15, 1676, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 16, 1682, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 17, 1688, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 18, 1694, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 19, 1700, property, remainingKeyBytes);
    C_008622e8_ProcessPropertyChunk(self, other, 20, 1704, property, remainingKeyBytes);
    NetPlayerDdl_ProcessBaseChunks(self, nullptr, other);
}

HOOK(0x00746890, C_008622e8_ProcessPlayerDdlChunks, "auto");
