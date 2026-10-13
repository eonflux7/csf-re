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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_Construct, 0x006F2AE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_BindCreatedChild, 0x006F2740, (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00862598_AllocateAndBindChild(C_00862598* self, void* /*edx*/)
{
    int rawValue = CoreMemory_AllocUntagged(0x54);
    if (rawValue != 0)
    {
        C_00862598* rawMemory = reinterpret_cast<C_00862598*>(static_cast<uintptr_t>(rawValue));
        uint32_t* createdObject = C_00862598_Construct(rawMemory, nullptr);
        C_00862598_BindCreatedChild(self, nullptr, createdObject);
        return createdObject;
    }

    C_00862598_BindCreatedChild(self, nullptr, nullptr);
    return nullptr;
}

HOOK(0x6F2B50, C_00862598_AllocateAndBindChild, "auto");
