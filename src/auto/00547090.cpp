#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// std::_String_base::assign-style helper (__thiscall, 3 stack args: src, pos, count; ret 12).
CSF_FUNC(void*, __fastcall, StringBase_AssignRange, 0x00488830,
         (void* self, void* edx, const void* src, uint32_t pos, uint32_t count));

// Initializes a string-like object at a1 from the pair at a2: copies the first dword into
// the object header, sets the size/capacity fields, clears the length word, and assigns
// the string body from a2+4 into the embedded string at a1+4. Returns the object pointer
// when non-null, otherwise returns the (null) argument. Stdcall, pops 8 bytes.
static void* __stdcall MenuExtras_InitStringFromPair(uint32_t* a1, uint32_t* a2)
{
    if (a1)
    {
        *a1 = *a2;
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(a1) + 28) = 7;
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(a1) + 24) = 0;
        *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(a1) + 8) = 0;
        return StringBase_AssignRange(reinterpret_cast<uint8_t*>(a1) + 4, nullptr,
                                      a2 + 1, 0u, 0xFFFFFFFFu);
    }
    return a1;
}

HOOK(0x00547090, MenuExtras_InitStringFromPair, "auto");
