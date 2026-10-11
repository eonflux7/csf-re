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

// Game heap allocator (cdecl, pops nothing itself; caller cleans the 4-byte argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor of the 32-byte node object allocated below; takes the owner graph as its int argument.
CSF_FUNC(uint32_t*, __fastcall, Waypoint_ConstructAllocatedNode, 0x0075FBF0, (void* self, void* edx, int a2));

static C_0086c288* __fastcall C_0086c288_Construct(C_0086c288* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0086C288);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0030 = 0;

    void* allocated = Mem_Alloc(32);
    if (allocated != nullptr)
    {
        uint32_t* node = Waypoint_ConstructAllocatedNode(allocated, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(self)));
        self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(node));
    }
    else
    {
        self->field_0014 = 0;
    }
    return self;
}

HOOK(0x0075E680, C_0086c288_Construct, "auto");
