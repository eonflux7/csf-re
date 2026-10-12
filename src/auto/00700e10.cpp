#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(float*, __cdecl, Water_ApplyTransform, 0x00803770, (int a1, uint16_t a2));

static int __fastcall C_008630e0_UpdatePlayerMatrices(
    C_008630e0* self, void* /*edx*/, int transforms)
{
    // This address is also listed as a vtable reference in the packet.
    (void)CSF_VTBL(0x006ADA10);

    const int playerCount = CSF_AT(
        std::uint16_t, reinterpret_cast<void*>(self->field_0004), 0x0C);

    alignas(8) std::uint8_t matrixBytes[0x10]{};
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

    int playerOffset = 0;
    for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex)
    {
        void* players = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0004), 0x10);
        matrix.field_0000 =
            CSF_AT(std::uint32_t, players, playerOffset + 8);
        CSF_AT(std::uint8_t, matrixBytes, 4) = 0;

        const bool applyWater =
            static_cast<std::uint8_t>(self->field_000c) != 0;

        if (!applyWater ||
            static_cast<std::uint8_t>(self->vSlot38()) != 0)
        {
            void* entries =
                CSF_AT(void*, reinterpret_cast<void*>(transforms), 4);
            auto* entry = static_cast<std::uint8_t*>(entries) + 8 * playerIndex;

            auto* row3 = CSF_AT(std::uint32_t*, entry, 0) + 12;
            auto* row2 = CSF_AT(std::uint32_t*, entry, 0) + 8;
            auto* row1 = CSF_AT(std::uint32_t*, entry, 0) + 4;
            auto* row0 = CSF_AT(std::uint32_t*, entry, 0);

            GfxMatrix_SetFourRowsAndUpdate(
                &matrix, nullptr, row0, row1, row2, row3);

            if (applyWater)
            {
                Water_ApplyTransform(
                    static_cast<int>(self->field_0004),
                    static_cast<std::uint16_t>(playerIndex));
            }
        }

        if (CSF_AT(std::uint8_t, matrixBytes, 4))
        {
            RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(matrix.field_0000));
        }

        playerOffset += 12;
    }

    return playerCount;
}

HOOK(0x00700E10, C_008630e0_UpdatePlayerMatrices, "auto");
