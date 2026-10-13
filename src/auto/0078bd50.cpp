#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee bindings (by address).
CSF_FUNC(char*, __fastcall, sub_78BC70, 0x0078BC70, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// __thiscall(char* this, uint32_t* a2), ret 4: copies the leading dword and the
// sub-object at a2+1, resets the length/capacity fields, and assigns a string
// substring from a2+34 (byte offset 136) into the member at this+136.
static char* __fastcall DataSetPerfCounters_Init(char* self, void* /*edx*/, std::uint32_t* a2)
{
    *reinterpret_cast<std::uint32_t*>(self) = a2[0];
    sub_78BC70(self + 4, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(a2 + 1)));

    CSF_AT(std::int32_t, self, 0xA0) = 15;
    CSF_AT(std::int32_t, self, 0x9C) = 0;
    CSF_AT(std::uint8_t, self, 0x8C) = 0;

    AssignStringSubstring(self + 0x88, nullptr,
                          reinterpret_cast<const void*>(a2 + 34),
                          0,
                          static_cast<std::uint32_t>(-1));
    return self;
}

HOOK(0x0078BD50, DataSetPerfCounters_Init, "auto");
