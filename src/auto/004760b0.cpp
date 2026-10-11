#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00862490_GetPosition(
    C_00862490* self, void* /*edx*/, std::uint32_t* position)
{
    auto* scene = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
    if (!scene)
        return 0;

    scene = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
    auto* transform = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, scene, 0)[0x18 / 4])(scene, nullptr);
    auto* source = ((std::uint32_t* (__fastcall*)(void*, void*))
        CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    position[0] = source[0];
    position[1] = source[1];
    const std::uint32_t result = source[2];
    position[2] = result;
    return static_cast<int>(result);
}

HOOK(0x004760B0, C_00862490_GetPosition, "auto");
