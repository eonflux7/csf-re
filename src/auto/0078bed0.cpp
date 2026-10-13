#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __stdcall, DataSetPerf_ConstructEntry, 0x0078BEB0, (char* a1, int a2));
CSF_FUNC(unsigned int, __stdcall, DataSetPerf_DestroyEntry, 0x0078BD00, (int a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Uninitialized copy of a range of 164-byte data set perf counter entries into dest.
// On exception, destroys the entries already constructed and rethrows.
// Returns the pointer one past the last constructed element.
static uint32_t* __cdecl DataSetPerf_UninitializedCopyEntries(uint32_t* first, uint32_t* last, uint32_t* dest, int unused, int allocator)
{
    char* start = reinterpret_cast<char*>(dest);
    char* out = start;
    try
    {
        while (first != last)
        {
            DataSetPerf_ConstructEntry(out, reinterpret_cast<int>(first));
            out += 164;
            first += 41;
        }
    }
    catch (...)
    {
        for (char* p = start; p != out; p += 164)
        {
            DataSetPerf_DestroyEntry(static_cast<int>(reinterpret_cast<uintptr_t>(p)));
        }
        RethrowCppException(nullptr, nullptr);
    }
    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x0078BED0, DataSetPerf_UninitializedCopyEntries, "auto");
