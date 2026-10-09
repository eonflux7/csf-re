#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Copies (via sub_4526B0) each 0x18-byte record from [first, last) into dest,
// advancing both pointers. On exception, stamps the vtable of C_0082d410 into the
// already-written records and rethrows.

CSF_FUNC(uint32_t*, __stdcall, CopyRecordElement, 0x004526B0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint32_t* __cdecl CopyRecordRange(uint32_t* first, uint32_t* last, uint32_t* dest, int /*a4*/, int /*a5*/)
{
    uint32_t* const destStart = dest;

    try
    {
        while (first != last)
        {
            CopyRecordElement(first, dest);
            dest += 6;   // 0x18 bytes per record
            first += 6;
        }
        return dest;
    }
    catch (...)
    {
        // Unwind cleanup: mark every record already written with C_0082d410's vtable.
        for (uint32_t* p = destStart; p != dest; p += 6)
        {
            p[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
            p[3] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
        }
        RethrowCppException(nullptr, nullptr);
        return dest;
    }
}

HOOK(0x00452760, CopyRecordRange, "auto");
