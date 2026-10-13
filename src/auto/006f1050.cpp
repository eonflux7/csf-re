#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_SetChildObject, 0x006CA030, (void* self, void* edx, uint32_t* child));
CSF_FUNC(int, __fastcall, SubObject_InitBase, 0x006CA660, (void* self, void* edx));

// Allocates a 60-byte child object, initializes its base, installs its vtables
// and zeroed fields, then attaches it to the owner. Returns the child or nullptr.
static uint32_t* __fastcall C_00862390_CreateChildObject(C_00862390* self, void* /*edx*/)
{
    uint8_t* child = static_cast<uint8_t*>(Mem_Alloc(60));
    if (child != nullptr)
    {
        SubObject_InitBase(child, nullptr);

        CSF_AT(void*, child, 0x00) = CSF_VTBL(0x00862390);
        CSF_AT(void*, child, 0x04) = CSF_VTBL(0x00862384);
        CSF_AT(void*, child, 0x08) = CSF_VTBL(0x00862368);

        CSF_AT(uint32_t, child, 0x20) = 0;
        CSF_AT(uint32_t, child, 0x24) = 0;
        CSF_AT(uint32_t, child, 0x28) = 0;
        CSF_AT(uint32_t, child, 0x2C) = 0;
        CSF_AT(uint32_t, child, 0x34) = 0;
        CSF_AT(uint32_t, child, 0x38) = 0;
        CSF_AT(uint32_t, child, 0x30) = 0;

        C_00862598_SetChildObject(self, nullptr, reinterpret_cast<uint32_t*>(child));
        return reinterpret_cast<uint32_t*>(child);
    }

    C_00862598_SetChildObject(self, nullptr, nullptr);
    return nullptr;
}

HOOK(0x006F1050, C_00862390_CreateChildObject, "auto");
