#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static int __fastcall MenuMessageBox_ApplyChildBounds(
    void* self, void* /*edx*/)
{
    void* container = CSF_AT(void*, self, 0x894);
    void* child = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, container, 0)[0x1CC / 4])(container, nullptr);

    if (!child)
        return 0;

    std::uint32_t bounds[4] = {0, 0, 0, 0};

    const std::uint32_t* position =
        ((const std::uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[0x20 / 4])(child, nullptr);

    bounds[0] = position[0];
    bounds[1] = position[1];

    const std::uint32_t* size =
        ((const std::uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr);

    bounds[2] = bounds[0] + size[0];
    const std::uint32_t bottom = bounds[1] + size[1];
    container = CSF_AT(void*, self, 0x894);
    bounds[3] = bottom;

    return ((int (__fastcall*)(void*, void*, std::uint32_t*, int))
        CSF_AT(void**, container, 0)[0x208 / 4])(
            container, nullptr, bounds, 0);
}

HOOK(0x00529590, MenuMessageBox_ApplyChildBounds, "auto");
