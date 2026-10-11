#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the object's 28-byte-entry table pointer stored at +0x0C (the Hex-Rays `*(this + 3)`).
// Entry 'index' holds a dword pointer to a polymorphic object. If that object is non-null,
// its virtual slot 20 (byte offset 0x50) is invoked with (a3, a4). Otherwise the function
// returns 28 * index.
static int __fastcall DispatchEntryVirtualSlot20(void* self, void* /*edx*/, int index, int a3, int a4)
{
    int result = 28 * index;

    uint32_t entryTable = CSF_AT(uint32_t, self, 0x0C);
    uint32_t entryObject = CSF_AT(uint32_t, reinterpret_cast<void*>(entryTable + 28 * index), 0);

    if (entryObject != 0)
    {
        void* obj = reinterpret_cast<void*>(entryObject);
        return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[0x50 / 4])(obj, nullptr, a3, a4);
    }
    return result;
}

HOOK(0x408490, DispatchEntryVirtualSlot20, "auto");
