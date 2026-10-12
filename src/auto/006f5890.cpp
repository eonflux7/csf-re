#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x1C bytes -> index 7 of the child's vtable.
// Child signature: __thiscall bool (int a2, int a3, int a4).
static bool __fastcall C_008627d8_QueryChildrenUntilHandled(C_008627d8* self, void* /*edx*/, int a2, int a3, int a4)
{
    bool result = false;
    uint32_t childCount = self->field_000c;
    if (childCount == 0)
        return result;

    uint32_t index = 0;
    do {
        uint32_t* childArray = reinterpret_cast<uint32_t*>(self->field_0004);
        void* child = reinterpret_cast<void*>(childArray[index]);
        auto childQuery = reinterpret_cast<bool(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, child, 0)[7]);
        if (childQuery(child, nullptr, a2, a3, a4)) {
            return true;
        }
        ++index;
    } while (index < childCount);

    return false;
}

HOOK(0x6F5890, C_008627d8_QueryChildrenUntilHandled, "auto");
