#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, DestroyElementRange, 0x00469760, (int* begin, int* end, int* owner, int* context));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int SceneLoader_DestroyStringRange(void* owner, int* context)
{
    int* begin = CSF_AT(int*, owner, 0x04);
    int result = 0;

    if (begin)
    {
        DestroyElementRange(begin, CSF_AT(int*, owner, 0x08),
                            reinterpret_cast<int*>(owner), context);

        int* block = CSF_AT(int*, owner, 0x04);
        result = static_cast<int>(reinterpret_cast<std::intptr_t>(block));
        if (block)
            result = Mem_FreeDebug(block);
    }

    CSF_AT(std::uint32_t, owner, 0x04) = 0;
    CSF_AT(std::uint32_t, owner, 0x08) = 0;
    CSF_AT(std::uint32_t, owner, 0x0C) = 0;
    return result;
}

static int __fastcall SceneLoader_DestroyStringRanges(void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);
    auto* context = reinterpret_cast<int*>(self);

    SceneLoader_DestroyStringRange(bytes + 0x80, context);
    SceneLoader_DestroyStringRange(bytes + 0x70, context);
    SceneLoader_DestroyStringRange(bytes + 0x60, context);
    SceneLoader_DestroyStringRange(bytes + 0x50, context);
    SceneLoader_DestroyStringRange(bytes + 0x40, context);
    SceneLoader_DestroyStringRange(bytes + 0x30, context);
    SceneLoader_DestroyStringRange(bytes + 0x20, context);
    SceneLoader_DestroyStringRange(bytes + 0x10, context);
    return SceneLoader_DestroyStringRange(self, context);
}

HOOK(0x00502E00, SceneLoader_DestroyStringRanges, "auto");
