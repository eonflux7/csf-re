#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static void __fastcall CMenuMessageBox_ForwardToChildren(
    CMenuMessageBox* self, void* /*edx*/, int value)
{
    const uint32_t childCount = self->field_095c;

    for (uint32_t index = 0; index < childCount; ++index) {
        // Reload the array pointer after each callback.
        auto* children = reinterpret_cast<uint32_t*>(self->field_0954);
        void* child = reinterpret_cast<void*>(children[index]);

        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, child, 0)[0xA8 / 4])(
                child, nullptr, value);
    }
}

HOOK(0x0052A7F0, CMenuMessageBox_ForwardToChildren, "auto");
