#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(void, __cdecl, NetChatDdl_ReadString, 0x00750EB0, (void* a1, void* a2));
CSF_FUNC(char, __fastcall, SpawnPoint_ApplyFallbackSelection, 0x00749420, (void* self, void* edx, int arg0, void* a3, void* a2, int a11));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __fastcall NetChatDdl_ApplySpawnPointFromPacket(
    C_00858180* self, void* /*edx*/, C_008622e8* stream)
{
    alignas(8) uint8_t stringBytes[0x1C];
    alignas(8) uint8_t firstValueBytes[0x4];
    alignas(8) uint8_t secondValueBytes[0x4];

    NetCogibleDdl_ReadIntoSlot(stream, nullptr,
        reinterpret_cast<int>(firstValueBytes));
    NetCogibleDdl_ReadIntoSlot(stream, nullptr,
        reinterpret_cast<int>(secondValueBytes));

    CSF_AT(uint32_t, stringBytes, 0x18) = 7;
    CSF_AT(int, stringBytes, 0x14) = 0;
    CSF_AT(uint16_t, stringBytes, 0x04) = 0;
    NetChatDdl_ReadString(stream, stringBytes);

    C_008622e8* counterSlot = stream;
    ReadCounterWord(stream, nullptr, reinterpret_cast<char*>(&counterSlot));

    char selectionResult = SpawnPoint_ApplyFallbackSelection(
        self, nullptr,
        CSF_AT(int, firstValueBytes, 0),
        reinterpret_cast<void*>(CSF_AT(uint32_t, secondValueBytes, 0)),
        stringBytes,
        reinterpret_cast<int>(counterSlot));

    void* result = reinterpret_cast<void*>(static_cast<intptr_t>(selectionResult));
    if (CSF_AT(uint32_t, stringBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, stringBytes, 0x04);
        if (result)
            return reinterpret_cast<void*>(static_cast<intptr_t>(Mem_FreeDebug(result)));
    }
    return result;
}

HOOK(0x007511B0, NetChatDdl_ApplySpawnPointFromPacket, "auto");
