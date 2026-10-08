#include <cstdint>
#include "globals.h"
#include "hook.h"

// std::_String_base *__thiscall sub_488830(...)  — real string constructor/assignment
// (this = destination string, then (source, a3, a4) on the stack; pops 12 bytes).
CSF_FUNC(void*, __fastcall, StringBase_ConstructFrom, 0x488830,
         (void* self, void* /*edx*/, void* source, unsigned int a3, unsigned int a4));

// Constructs a string sub-object at pDest out of (source, a3, a4).
// Inline prefix: clears three members, then delegates to the real constructor.
static int __fastcall CMenuEula_ConstructStringCopy(void* self, void* /*edx*/,
                                                    char* pDest, unsigned int a3, unsigned int a4)
{
    CSF_AT(uint32_t, pDest, 0x14) = 0;
    CSF_AT(uint32_t, pDest, 0x18) = 7;
    CSF_AT(uint16_t, pDest, 0x04) = 0;

    StringBase_ConstructFrom(pDest, nullptr, self, a3, a4);

    return reinterpret_cast<int>(pDest);
}

HOOK(0x533530, CMenuEula_ConstructStringCopy, "auto");
