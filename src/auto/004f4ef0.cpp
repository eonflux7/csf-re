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
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));

// Sets the provider's position through its scene-graph matrix, then releases the
// matrix returned by the provider when the output flag is set.
static char __fastcall C_0085ec18_SetProviderPosition(C_0085ec18* self, void* /*edx*/, int a2)
{
    // The provider object held in field_0004; slot 4 returns its gfx-node object.
    void* providerObj = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0004));
    void* gfxNodeObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, providerObj, 0)[4])(providerObj, nullptr);

    // Output slot (ebp-8: matrix pointer, ebp-4: flag byte). Callee fills it.
    alignas(8) uint8_t localBytes[8];

    // Slot 35 takes the output pointer and returns the matrix-holder to scale.
    void* matrixHolder = ((void* (__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, gfxNodeObj, 0)[35])(
        gfxNodeObj, nullptr, reinterpret_cast<uint32_t*>(localBytes));

    // a2 stays on the stack and is consumed here as the source pointer.
    GfxMatrix_SetScaleAndUpdate(matrixHolder, nullptr,
        reinterpret_cast<const uint32_t*>(static_cast<std::uintptr_t>(a2)));

    char flag = CSF_AT(char, localBytes, 4);
    if (flag)
    {
        uint32_t matrixPtr = *reinterpret_cast<uint32_t*>(localBytes);
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(matrixPtr))));
    }
    return flag;
}

HOOK(0x004F4EF0, C_0085ec18_SetProviderPosition, "auto");
