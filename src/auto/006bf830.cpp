#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static int __fastcall C_00860700_AttachFrame(
    C_00860700* self, void* /*edx*/, int childAddress, char preserveWorldTransform)
{
    auto* child = reinterpret_cast<C_00860700*>(childAddress);

    if (preserveWorldTransform)
    {
        alignas(8) std::uint8_t childMatrixBytes[0x8];
        alignas(8) std::uint8_t parentMatrixBytes[0x8];
        alignas(8) std::uint8_t inverseMatrixBytes[0xC];

        child->vSlot8C(reinterpret_cast<int>(childMatrixBytes));

        CSF_AT(std::uint32_t*, inverseMatrixBytes, 0) = RwMatrixCreate();
        CSF_AT(std::uint8_t, inverseMatrixBytes, 4) = 1;

        auto* parentMatrix = static_cast<S_006adb40*>(
            self->vSlot8C(reinterpret_cast<int>(parentMatrixBytes)));

        // The pending &inverseMatrixBytes argument belongs to the inversion,
        // not to the preceding one-argument virtual getter.
        // The packet also lists this call address as a vtable reference.
        (void)CSF_VTBL(0x006ADB40);
        Matrix_InvertWithStoredTarget(
            parentMatrix, nullptr, reinterpret_cast<float**>(inverseMatrixBytes));

        if (CSF_AT(std::uint8_t, parentMatrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, parentMatrixBytes, 0));

        std::uint32_t* relativeMatrix = RwMatrixCreate();
        RwMatrixMultiply(
            relativeMatrix,
            CSF_AT(std::uint32_t*, childMatrixBytes, 0),
            CSF_AT(std::uint32_t*, inverseMatrixBytes, 0));

        CSF_AT(std::uint32_t*, parentMatrixBytes, 0) = RwMatrixCreate();
        std::memcpy(
            CSF_AT(std::uint32_t*, parentMatrixBytes, 0), relativeMatrix, 0x40);
        CSF_AT(std::uint8_t, parentMatrixBytes, 4) = 1;
        RwMatrixDestroy(relativeMatrix);

        child->vSlot98(reinterpret_cast<void**>(parentMatrixBytes));

        if (CSF_AT(std::uint8_t, parentMatrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, parentMatrixBytes, 0));
        if (CSF_AT(std::uint8_t, inverseMatrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrixBytes, 0));
        if (CSF_AT(std::uint8_t, childMatrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, childMatrixBytes, 0));
    }
    else
    {
        child->vSlot1C(reinterpret_cast<std::uint32_t*>(&g_ZeroVectorX));
        child->vSlot4C(reinterpret_cast<std::uint32_t*>(&g_ZeroVectorX));
    }

    return self->vSlot7C(childAddress);
}

HOOK(0x006BF830, C_00860700_AttachFrame, "auto");
