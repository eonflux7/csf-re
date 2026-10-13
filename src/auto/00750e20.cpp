#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(char, __fastcall, SpawnPoint_HandleMultiplayerRequest, 0x00749510, (void* self, void* edx, int arg0, int a3, int a4, int a5, int a6, int a7, uint16_t a2));

static char __fastcall C_00858180_ReadAndDispatchSpawnRequest(
    C_00858180* self, void* /*edx*/, C_008622e8* reader)
{
    int spawnIndex = 0;
    int v6 = 0;
    int v7 = 0;
    int v8 = 0;
    int v9 = 0;
    int v10 = 0;

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&spawnIndex));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&v6));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&v7));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&v8));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&v9));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&v10));

    // ReadCounterWord may rewrite the reader slot (the original's stack argument).
    C_008622e8* readerSlot = reader;
    ReadCounterWord(reader, nullptr, reinterpret_cast<char*>(&readerSlot));

    return SpawnPoint_HandleMultiplayerRequest(
        self, nullptr, spawnIndex, v6, v7, v8, v9, v10,
        static_cast<uint16_t>(reinterpret_cast<uintptr_t>(readerSlot)));
}

HOOK(0x00750E20, C_00858180_ReadAndDispatchSpawnRequest, "auto");
