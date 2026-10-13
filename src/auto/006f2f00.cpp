#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxRenderEngine_ApplyParticleTransform, 0x006EF1E0, (S_006ada10* self, void* , const void** a2));

static char __fastcall C_00862640_UpdateParticleTransform(
    C_00862640* self, void* /*edx*/)
{
    alignas(8) std::uint8_t sourceTransformBytes[0x8];
    alignas(8) std::uint8_t transformBytes[0x8];

    void* owner = reinterpret_cast<void*>(self->field_0024);
    void** particleVtable =
        CSF_AT(void**, reinterpret_cast<void*>(self->field_0020), 0);

    int ownerValue =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x10 / 4])(owner, nullptr);

    reinterpret_cast<void (__fastcall*)(S_006ada10*, void*, int)>(
        particleVtable[0x20 / 4])(
            reinterpret_cast<S_006ada10*>(self->field_0020),
            nullptr, ownerValue);

    owner = reinterpret_cast<void*>(self->field_0024);
    void* transformResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, owner, 0)[0x8C / 4])(
                owner, nullptr, transformBytes);
    void* matrix = CSF_AT(void*, transformResult, 0);

    self->field_0028 = CSF_AT(float, matrix, 0x20);
    CSF_AT(float, self, 0x2C) = CSF_AT(float, matrix, 0x24);
    CSF_AT(float, self, 0x30) = CSF_AT(float, matrix, 0x28);

    char result = CSF_AT(char, transformBytes, 0x4);
    if (result)
        result = static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, transformBytes, 0)));

    if (CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(self->field_0018), 0x58) != 1)
    {
        std::uint32_t* copiedMatrix = RwMatrixCreate();
        owner = reinterpret_cast<void*>(self->field_0024);
        CSF_AT(std::uint32_t*, transformBytes, 0) = copiedMatrix;
        CSF_AT(char, transformBytes, 0x4) = 1;

        void* sourceResult =
            reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, owner, 0)[0x8C / 4])(
                    owner, nullptr, sourceTransformBytes);

        const void* sourceMatrix = CSF_AT(const void*, sourceResult, 0);
        std::memcpy(CSF_AT(void*, transformBytes, 0), sourceMatrix, 0x40);

        if (CSF_AT(char, sourceTransformBytes, 0x4))
            RwMatrixDestroy(
                CSF_AT(std::uint32_t*, sourceTransformBytes, 0));

        GfxRenderEngine_ApplyParticleTransform(
            reinterpret_cast<S_006ada10*>(self->field_0020),
            nullptr, reinterpret_cast<const void**>(transformBytes));

        result = CSF_AT(char, transformBytes, 0x4);
        if (result)
            return static_cast<char>(
                RwMatrixDestroy(CSF_AT(std::uint32_t*, transformBytes, 0)));
    }

    return result;
}

HOOK(0x006F2F00, C_00862640_UpdateParticleTransform, "auto");
