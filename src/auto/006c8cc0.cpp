#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, RecordObject_Construct, 0x006C2000, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, RecordObject_Cleanup, 0x006C2CF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, RecordObject_InitFromSource, 0x006C3290, (void* self, void* edx, uint8_t* a2, uint8_t* a3));

// Allocates a 36-byte record, constructs it, and initializes it from the source.
// On init failure the record is cleaned up and freed; returns the record or nullptr.
static void* __stdcall CreateRecordFromSource(uint8_t* a2, uint8_t* a3)
{
    void* result = nullptr;
    if (a2 && *a2)
    {
        void* block = Mem_Alloc(36);
        void* record = nullptr;
        if (block)
        {
            // RecordObject_Construct installs the record vtable (CSF_VTBL(0x006C2000)).
            (void)CSF_VTBL(0x006C2000);
            record = RecordObject_Construct(block, nullptr);
        }

        if (RecordObject_InitFromSource(record, nullptr, a2, a3) || !record)
        {
            return record;
        }

        RecordObject_Cleanup(record, nullptr);
        Mem_Free(record);
        return nullptr;
    }
    return result;
}

HOOK(0x006C8CC0, CreateRecordFromSource, "auto");
