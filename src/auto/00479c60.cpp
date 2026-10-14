#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static int __fastcall C_00862490_ActivateSlotEntry(
    C_00862490* self, void* /*edx*/)
{
    void* entry =
        reinterpret_cast<void* (__fastcall*)(C_00862490*, void*)>(
            self->vtbl[0xA8 / 4])(self, nullptr);

    if (!entry)
        return 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, entry, 0)[0x34 / 4])(entry, nullptr, 1);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, 1);

    if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA8 / 4])(entry, nullptr))
    {
        struct MatrixState
        {
            std::uint32_t* matrix;
            std::uint8_t ownsMatrix;
            std::uint8_t padding[3];
        };
        static_assert(sizeof(MatrixState) == 0x8);
        MatrixState matrixState;

        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, MatrixState*)>(
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, &matrixState);

        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA4 / 4])(entry, nullptr);

        transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, MatrixState*)>(
            CSF_AT(void**, transform, 0)[0x94 / 4])(
                transform, nullptr, &matrixState);

        if (matrixState.ownsMatrix)
            RwMatrixDestroy(matrixState.matrix);
    }

    void* owner = reinterpret_cast<void*>(self->field_0004);
    const bool ownerFlag = CSF_AT(std::uint8_t, owner, 0x228) != 0;

    void* slotManager =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    return reinterpret_cast<int (__fastcall*)(void*, void*, void*, int, int)>(
        CSF_AT(void**, slotManager, 0)[0xA0 / 4])(
            slotManager, nullptr, entry, ownerFlag ? 60 : -1,
            ownerFlag ? 0 : 1);
}

HOOK(0x00479C60, C_00862490_ActivateSlotEntry, "auto");
