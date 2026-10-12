#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int*, __fastcall, sub_43F780, 0x0043F780, (uint32_t* a1, int a2, int* a3, int a4, int a5));
CSF_FUNC(int*, __fastcall, sub_440DE0, 0x00440DE0, (void* self, void* edx, int a2));

static int* __fastcall SceneLoader_ResetPreviousSceneManager(void* self, void* /*edx*/)
{
    auto* base = static_cast<uint8_t*>(self);
    auto* listNode = base + 4;
    auto* sentinel = base + 0x10;

    if (sentinel != listNode)
    {
        int* headPointer = CSF_AT(int*, base, 0x14);
        int headValue = CSF_AT(int, headPointer, 0);

        alignas(8) uint8_t resultBytes[0x4];
        sub_43F780(
            reinterpret_cast<uint32_t*>(sentinel),
            0,
            reinterpret_cast<int*>(resultBytes),
            headValue,
            static_cast<int>(reinterpret_cast<uintptr_t>(headPointer)));

        return sub_440DE0(
            sentinel, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(listNode)));
    }

    return nullptr;
}

HOOK(0x00441990, SceneLoader_ResetPreviousSceneManager, "auto");
