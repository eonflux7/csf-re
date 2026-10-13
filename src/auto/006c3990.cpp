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

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_SetRotationAroundX, 0x006AE250, (S_006ada10* self, void* /*edx*/, float angle));
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* /*edx*/, float angle));
CSF_FUNC(int, __fastcall, GfxTeam_DrawStringPass, 0x006C3820, (void* self, void* edx, float* a2, float a3, std::uint8_t* a4, int a5));

// The drawing helper's class has no vtable; the packet lists this reference at each call site.
[[maybe_unused]] static const auto kStringPassVtbl = CSF_VTBL(0x006C3820);

static char __fastcall GfxTeam_DrawStringMarker(void* self, void* /*edx*/, float* a2, float a3, std::uint8_t* a4)
{
    // Stack frame of the temporary matrix object (8 bytes):
    //   +0: matrix pointer (from RwMatrixCreate)
    //   +4: owned flag (set to 1 before the first pass)
    alignas(8) std::uint8_t matrixFrame[8];

    CSF_AT(std::uint32_t*, matrixFrame, 0x0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixFrame, 0x4) = 1;
    GfxTeam_DrawStringPass(self, nullptr, a2, a3, a4,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_AT(std::uint32_t*, matrixFrame, 0x0))));

    GfxMatrix_SetRotationAroundX(reinterpret_cast<S_006ada10*>(matrixFrame), nullptr, 1.5707964f);
    GfxTeam_DrawStringPass(self, nullptr, a2, a3, a4,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_AT(std::uint32_t*, matrixFrame, 0x0))));

    GfxMatrix_SetYRotation(reinterpret_cast<S_006ada10*>(matrixFrame), nullptr, 1.5707964f);
    GfxTeam_DrawStringPass(self, nullptr, a2, a3, a4,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_AT(std::uint32_t*, matrixFrame, 0x0))));

    std::uint8_t result = CSF_AT(std::uint8_t, matrixFrame, 0x4);
    if (result)
        return static_cast<char>(RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixFrame, 0x0)));
    return static_cast<char>(result);
}

HOOK(0x006C3990, GfxTeam_DrawStringMarker, "auto");
