#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00862240_Construct, 0x006F0040, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_SetChildObject, 0x006CA030, (C_00862598* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00862240_AllocateAndBindChild(C_00862240* self, void* /*edx*/)
{
    int rawAddress = CoreMemory_AllocUntagged(208);
    if (rawAddress)
    {
        void* rawMemory = reinterpret_cast<void*>(static_cast<std::uintptr_t>(rawAddress));
        uint32_t* createdObject = C_00862240_Construct(rawMemory, nullptr);
        C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr, createdObject);
        return createdObject;
    }

    C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr, nullptr);
    return nullptr;
}

HOOK(0x6F0170, C_00862240_AllocateAndBindChild, "auto");
