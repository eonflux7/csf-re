#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __stdcall, WeaponDefs_CopyConstructEntry, 0x00784290, (char* a1, uint32_t* a2));
CSF_FUNC(int, __stdcall, WeaponDefs_DestroyEntry, 0x00783620, (int a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Uninitialized copy of a range of 552-byte weapon entries into dest.
// On exception, destroys the entries already constructed and rethrows.
// Returns the pointer one past the last constructed element.
static uint32_t* __cdecl WeaponDefs_UninitializedCopyEntries(uint32_t* first, uint32_t* last, uint32_t* dest, int unused, int allocator)
{
    char* start = reinterpret_cast<char*>(dest);
    char* out = start;
    try
    {
        while (first != last)
        {
            WeaponDefs_CopyConstructEntry(out, first);
            out += 552;
            first += 138;
        }
    }
    catch (...)
    {
        for (char* p = start; p != out; p += 552)
        {
            WeaponDefs_DestroyEntry(reinterpret_cast<int>(p));
        }
        RethrowCppException(nullptr, nullptr);
    }
    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x007842B0, WeaponDefs_UninitializedCopyEntries, "auto");
