#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* /*edx*/, int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));
CSF_FUNC(void*, __fastcall, C_00861bd8_InitWithOwner, 0x006E5210, (void* self, void* edx, void** a2, char a3));

// Returns the payload stored under key 5 of the keyed table owned by field_0040,
// creating and registering a new C_00861bd8 payload when none exists yet.
static void* __fastcall C_00861ce0_GetOrCreateKey5Payload(C_00861ce0* self, void* /*edx*/)
{
    // Look up key 5 in the keyed table (virtual slot 0x1C with one stack argument: key 5).
    void* keyTableOwner = reinterpret_cast<void*>(self->field_0040);
    auto getKeyedTable = reinterpret_cast<C_0085e7b8*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, keyTableOwner, 0)[7]);
    C_0085e7b8* lookupTable = getKeyedTable(keyTableOwner, nullptr, 5);

    int foundEntry = C_0085e7b8_FindEntryPayloadByKey(lookupTable, nullptr, 5);
    if (foundEntry != 0)
    {
        void** entryPayload = reinterpret_cast<void**>(static_cast<std::intptr_t>(foundEntry));
        void* existingPayload = *entryPayload;
        if (existingPayload != nullptr)
            return existingPayload;
    }

    // Determine whether the flagged object has bit 0x800 set.
    bool isFlagged = false;
    if (self->field_0010 != 0)
    {
        void* flagsObject = reinterpret_cast<void*>(self->field_0010);
        auto getFlagsWord = reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, flagsObject, 0)[8]);
        uint32_t* flagsWord = getFlagsWord(flagsObject, nullptr);
        isFlagged = (*flagsWord & 0x800u) == 0x800u;
    }

    // Fetch the table to register into (virtual slot 0x1C, no arguments).
    void* tableOwner = reinterpret_cast<void*>(self->field_0040);
    auto getTable = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, tableOwner, 0)[7]);
    void* table = getTable(tableOwner, nullptr);

    void* allocated = Mem_Alloc(20);
    if (allocated != nullptr)
    {
        void* payload = C_00861bd8_InitWithOwner(
            allocated, nullptr, reinterpret_cast<void**>(table), static_cast<char>(!isFlagged));
        C_008670f0_SetEntry(reinterpret_cast<C_008670f0*>(table), nullptr, 5,
            static_cast<int>(reinterpret_cast<std::intptr_t>(payload)));
        return payload;
    }

    C_008670f0_SetEntry(reinterpret_cast<C_008670f0*>(table), nullptr, 5, 0);
    return nullptr;
}

HOOK(0x006E5580, C_00861ce0_GetOrCreateKey5Payload, "auto");
