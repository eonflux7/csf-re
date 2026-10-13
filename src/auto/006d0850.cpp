#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, CCamera_Construct, 0x006D32F0, (CCamera* self, void*));
CSF_FUNC(int, __fastcall, C_008655e8_Setup, 0x006D3450, (C_008655e8* self, void*, int setupAddress, uint32_t context));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void*, int additionalCapacity));

// Allocates a 228-byte camera object, constructs it, runs its setup, and appends it
// to the array embedded at this+4. Returns the camera pointer (0 if allocation failed).
static int __fastcall C_00860db0_CreateCameraAndAppend(C_00860db0* self, void* /*edx*/, int setupAddress)
{
    // Vtable reference to S_0041e5f0 (its vtable is used by the grow call path).
    (void)CSF_VTBL(0x0041E5F0);

    int allocated = CoreMemory_AllocUntagged(228);
    C_008655e8* camera = nullptr;
    if (allocated != 0)
        camera = reinterpret_cast<C_008655e8*>(CCamera_Construct(reinterpret_cast<CCamera*>(allocated), nullptr));

    C_008655e8_Setup(camera, nullptr, setupAddress, self->field_0024);

    // Embedded array object at this+4.
    auto* arrayObj = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 4);
    if (self->field_000c >= self->field_0008)
        S_0041e5f0_GrowArrayCapacity(arrayObj, nullptr, arrayObj->field_000c);

    uint32_t index = arrayObj->field_0008;
    C_008655e8** slot = reinterpret_cast<C_008655e8**>(arrayObj->field_0000 + 4 * index);
    arrayObj->field_0008 = index + 1;
    *slot = camera;

    return static_cast<int>(reinterpret_cast<uintptr_t>(camera));
}

HOOK(0x006D0850, C_00860db0_CreateCameraAndAppend, "auto");
