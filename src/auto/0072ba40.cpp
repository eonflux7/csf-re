#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_DispatchToChildrenReverse(
    CGuiPanel* self, void* /*edx*/, int argument)
{
    int result = 0;
    const std::uint32_t childrenBegin = self->field_0138;
    if (childrenBegin != 0) {
        result = static_cast<std::int32_t>(
            self->field_013c - childrenBegin) >> 2;
    }

    for (int index = result - 1; index >= 0; --index) {
        void* child = reinterpret_cast<void*>(
            reinterpret_cast<const std::uint32_t*>(
                self->field_0138)[index]);

        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, child, 0)[0xA8 / 4])(
                child, nullptr, argument);
    }

    return result;
}

HOOK(0x0072BA40, CGuiPanel_DispatchToChildrenReverse, "auto");
