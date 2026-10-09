#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(unsigned int, __fastcall, sub_49C1D0, 0x0049C1D0, (void* self, void* edx, uint32_t* a3));

static int __fastcall ForwardToSubobjectAndSetFlag200(void* self, void* /*edx*/, int a2)
{
    // Forward the argument to the embedded subobject at +0x2F8, then set bit 0x200 in the flags at +0x2E0.
    int result = (int)sub_49C1D0((uint8_t*)self + 0x2F8, nullptr,
                                 reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2)));
    CSF_AT(uint32_t, self, 0x2E0) |= 0x200u;
    return result;
}

HOOK(0x0049C7F0, ForwardToSubobjectAndSetFlag200, "auto");
