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

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Slot 0x10 of the provider's vtable: returns the source object (a transform/matrix owner).
using GetSourceObjectFn = void* (__fastcall*)(void* self, void* edx);

// Slot 0x8C of the source object's vtable: fills an 8-byte out buffer
// (dword matrix handle at +0, owned-flag byte at +4) and returns a pointer to the position dword.
using GetPositionMatrixFn = std::uint32_t* (__fastcall*)(void* self, void* edx, std::uint32_t* outMatrix);

static int __fastcall MpSpawnPoints_GetProviderPosition(C_0085ec18* self, void* /*edx*/)
{
    void* providerObject = reinterpret_cast<void*>(self->field_0004);
    void* sourceObject = reinterpret_cast<GetSourceObjectFn>(CSF_AT(void**, providerObject, 0)[0x10 / 4])(providerObject, nullptr);

    // Stack buffer the callee fills: [0..3] matrix handle, [4] ownership flag.
    alignas(8) std::uint8_t outBuffer[8] = {};
    std::uint32_t& matrixHandle = *reinterpret_cast<std::uint32_t*>(outBuffer);

    GetPositionMatrixFn getPositionMatrix =
        reinterpret_cast<GetPositionMatrixFn>(CSF_AT(void**, sourceObject, 0)[0x8C / 4]);
    std::uint32_t* positionPtr = getPositionMatrix(sourceObject, nullptr, &matrixHandle);

    int result = static_cast<int>(*positionPtr) + 0x20;

    std::uint8_t ownsMatrix = outBuffer[4];
    if (ownsMatrix)
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));

    return result;
}

HOOK(0x004F4EB0, MpSpawnPoints_GetProviderPosition, "auto");
