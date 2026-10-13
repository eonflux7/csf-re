#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00862640_Construct, 0x006F2D90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_SetChildObject, 0x006CA030, (C_00862598* self, void* /*edx*/, uint32_t* a2));

static uint32_t* __fastcall C_00862640_AllocateAndBindChild(C_00862640* self, void* /*edx*/)
{
    int rawMemory = CoreMemory_AllocUntagged(84);
    if (rawMemory)
    {
        void* rawObject = reinterpret_cast<void*>(rawMemory);
        uint32_t* createdObject = C_00862640_Construct(rawObject, nullptr);
        C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr, createdObject);
        return createdObject;
    }

    C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr, nullptr);
    return nullptr;
}

HOOK(0x6F3340, C_00862640_AllocateAndBindChild, "auto");
