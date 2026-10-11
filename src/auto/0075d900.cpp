#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_75D4B0: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(char, __fastcall, WaypointGraph_InsertIntoContainer, 0x0075D4B0,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

static uint32_t* __fastcall S_0075d900_InsertAndGetIterator(
    S_0075d900* self, void* /*edx*/, uint32_t* result,
    char* position, uint32_t* extraArg)
{
    uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 44 != 0) {
            index = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin) / 44;
        }
    }

    WaypointGraph_InsertIntoContainer(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 44u;
    return result;
}

HOOK(0x0075D900, S_0075d900_InsertAndGetIterator, "auto");
