#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static char __fastcall CharacterModel_ApplyConfiguration(
    void* self, void* /*edx*/, void* configuration)
{
    void* model = CSF_AT(void*, self, 0x10);
    void* controller =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, model, 0)[0x68 / 4])(
            model, nullptr);

    ((void (__fastcall*)(void*, void*, int, int, void*))
        CSF_AT(void**, controller, 0)[0x14 / 4])(
            controller, nullptr, 0, 0, configuration);

    if (CSF_AT(std::int32_t, self, 0x0C) == 1 &&
        CSF_AT(std::uint32_t, self, 0x08) == 0 &&
        CSF_AT(std::int32_t, self, 0x7C) >= 0)
    {
        alignas(8) std::uint8_t matrixBytes[0x8] = {};

        model = CSF_AT(void*, self, 0x10);
        void* transform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, model, 0)[0x18 / 4])(
                model, nullptr);

        CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
        CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x98 / 4])(
                transform, nullptr, matrixBytes);
        if (CSF_AT(std::uint8_t, matrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

        void* secondaryModel = CSF_AT(void*, self, 0x04);
        transform =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, secondaryModel, 0)[0x10 / 4])(
                    secondaryModel, nullptr);

        CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
        CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x98 / 4])(
                transform, nullptr, matrixBytes);
        if (CSF_AT(std::uint8_t, matrixBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));
    }

    CSF_AT(std::int32_t, self, 0x14) = -1;
    const char state = CSF_AT(char, configuration, 0x0C);
    CSF_AT(char, self, 0x18) = state;
    return state;
}

HOOK(0x004F5310, CharacterModel_ApplyConfiguration, "auto");
