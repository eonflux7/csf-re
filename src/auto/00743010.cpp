#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, ResolveObjectEntry, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static int __fastcall NetCogibleDDL_ProcessChunks(char* self, void* /*edx*/, C_008622e8* arg)
{
    void* entry;
    // 4-byte stack slot ([ebp-4]) holding the first chunk id
    alignas(4) uint8_t firstChunkSlot[4];
    firstChunkSlot[0] = 1;
    C_008622e8* a = arg;
    // later chunk ids live in the high byte of the argument slot ([ebp+0xb])
    uint8_t* argHighByte = reinterpret_cast<uint8_t*>(&arg) + 3;

    {
        uint8_t* obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2c) >> 22));
        ResolveObjectEntry(obj + 0xc, nullptr, &entry, firstChunkSlot);
        a = arg;
        void* target = CSF_AT(void*, entry, 0x10);
        ProcessDDLChunk(target, nullptr, (int)self, (int)(self + 0x44), 1, a);
    }

    {
        *argHighByte = 2;
        uint8_t* obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2c) >> 22));
        ResolveObjectEntry(obj + 0xc, nullptr, &entry, argHighByte);
        void* target = CSF_AT(void*, entry, 0x10);
        ProcessDDLChunk(target, nullptr, (int)self, (int)(self + 0x6c), 2, a);
    }

    {
        *argHighByte = 3;
        uint8_t* obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2c) >> 22));
        ResolveObjectEntry(obj + 0xc, nullptr, &entry, argHighByte);
        void* target = CSF_AT(void*, entry, 0x10);
        ProcessDDLChunk(target, nullptr, (int)self, (int)(self + 0x6f), 3, a);
    }

    {
        *argHighByte = 4;
        uint8_t* obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2c) >> 22));
        ResolveObjectEntry(obj + 0xc, nullptr, &entry, argHighByte);
        void* target = CSF_AT(void*, entry, 0x10);
        ProcessDDLChunk(target, nullptr, (int)self, (int)(self + 0x72), 4, a);
    }

    C_0085ec18_NullSub((int)a);
    return 0;
}

HOOK(0x00743010, NetCogibleDDL_ProcessChunks, "auto");
