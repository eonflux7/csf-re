#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, sub_69A8A0, 0x0069A8A0, (void* self, void* edx, void** a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, sub_69AB30, 0x0069AB30, (void* self, void* edx, int a2, uint32_t* a3));

static int** __fastcall SceneLoader_RegisterArchivedObject(
    S_0069a8a0* self, void* /*edx*/, int objectId)
{
    static_cast<void>(CSF_VTBL(0x0069A8A0));

    alignas(8) uint8_t frameBytes[0x10];

    void** result = sub_69A8A0(
        self, nullptr,
        reinterpret_cast<void**>(frameBytes + 0xC),
        reinterpret_cast<uint32_t*>(&objectId));

    if (CSF_AT(void*, frameBytes, 0xC) ==
        reinterpret_cast<void*>(self->field_0004))
    {
        CSF_AT(uint32_t, frameBytes, 0x8) =
            static_cast<uint32_t>(objectId);
        CSF_AT(uint8_t, frameBytes, 0xC) = 0;

        return reinterpret_cast<int**>(sub_69AB30(
            self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(frameBytes)),
            reinterpret_cast<uint32_t*>(frameBytes + 0x8)));
    }

    return reinterpret_cast<int**>(result);
}

HOOK(0x0069ACB0, SceneLoader_RegisterArchivedObject, "auto");
