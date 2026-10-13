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

CSF_FUNC(std::uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int type));

// Creates a particle object from the spec's first dword, binds the spec to it,
// then activates it (vtable slot 1 with argument 1) and returns that call's result.
static int __stdcall ParticleSpawn_CreateAndActivate(std::uint32_t* a1)
{
    std::uint32_t* particle = CreateParticleObject(static_cast<int>(a1[0]));
    if (particle != nullptr)
    {
        // vtable slot 0x18 / 4 = 6: bind(spec)
        ((void(__fastcall*)(void*, void*, std::uint32_t*))CSF_AT(void**, particle, 0)[6])(particle, nullptr, a1);
        // vtable slot 4 / 4 = 1: activate(1)
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, particle, 0)[1])(particle, nullptr, 1);
    }
    return 0;
}

HOOK(0x006DF900, ParticleSpawn_CreateAndActivate, "auto");
