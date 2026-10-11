#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// __thiscall with one stack argument (a2), callee pops 4 bytes.
static void __fastcall S_004f5420_SetStateRefreshingMatrix(S_004f5420* self, void* /*edx*/, int a2)
{
    void* renderObject = reinterpret_cast<void*>(self->field_0010);
    if (renderObject == nullptr)
    {
        self->field_000c = static_cast<uint32_t>(a2);
        return;
    }

    if (a2 == 2 || (a2 == 1 && self->field_000c != static_cast<uint32_t>(a2)))
    {
        // Slot 0x18 / 4 = 6 on the render object; returns the matrix owner.
        void* matrixOwner = ((void* (__fastcall*)(void*, void*))(CSF_AT(void**, renderObject, 0)[6]))(renderObject, nullptr);

        // Stack object {uint32 matrix; uint8 ownsMatrix} at [ebp-8]; the callee may clear the flag.
        alignas(8) uint8_t matrixHolder[8];
        CSF_AT(std::uint32_t*, matrixHolder, 0) = RwMatrixCreate();
        CSF_AT(uint8_t, matrixHolder, 4) = 1;

        // Slot 0x98 / 4 = 38 on the matrix owner, one stack argument: &matrix holder.
        ((void (__fastcall*)(void*, void*, void*))(CSF_AT(void**, matrixOwner, 0)[38]))(matrixOwner, nullptr, matrixHolder);

        if (CSF_AT(uint8_t, matrixHolder, 4) != 0)
        {
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHolder, 0));
        }
    }

    self->field_000c = static_cast<uint32_t>(a2);
}

HOOK(0x004F5420, S_004f5420_SetStateRefreshingMatrix, "auto");
