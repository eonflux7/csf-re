#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Grows the SpawnPoints entry buffer. __fastcall-style: (self, edx, extraEntries), callee pops nothing.
CSF_FUNC(void*, __fastcall, SpawnPoints_GrowEntryBuffer, 0x00551CF0, (void* self, void* , int extraEntries));

// Appends one (value, packed flag bytes) entry to the action list embedded at this+0x28.
// Stack args: a2 (int), a3 (char), a4 (char); ret 12.
static int __fastcall TCharTask_AppendActionEntry(void* self, void* /*edx*/, int a2, char a3, char a4)
{
    uint8_t* list = reinterpret_cast<uint8_t*>(self) + 0x28;

    // Packed low two bytes: a3 in byte 0, a4 in byte 1. Upper bytes are not defined by the original; use 0.
    uint32_t packedFlags = static_cast<uint32_t>(static_cast<uint8_t>(a3))
                         | (static_cast<uint32_t>(static_cast<uint8_t>(a4)) << 8);

    // list+8 = used count, list+4 = capacity, list+0xC = grow amount, list+0 = entry buffer.
    if (static_cast<uint32_t>(CSF_AT(uint32_t, list, 0x8)) >= static_cast<uint32_t>(CSF_AT(uint32_t, list, 0x4)))
    {
        SpawnPoints_GrowEntryBuffer(list, nullptr, static_cast<int>(CSF_AT(uint32_t, list, 0xC)));
    }

    uint32_t index = CSF_AT(uint32_t, list, 0x8);
    uint32_t bufferAddress = CSF_AT(uint32_t, list, 0x0);
    uint8_t* entry = reinterpret_cast<uint8_t*>(bufferAddress) + index * 8;

    CSF_AT(uint32_t, list, 0x8) = index + 1;
    CSF_AT(uint32_t, entry, 0x0) = static_cast<uint32_t>(a2);
    CSF_AT(uint32_t, entry, 0x4) = packedFlags;

    return static_cast<int>(packedFlags);
}

HOOK(0x0048CFE0, TCharTask_AppendActionEntry, "auto");
