#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall WeaponFirearms_UpdateContextFromObject(
    void* self, void* /*edx*/, void* object, float duration)
{
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t matrixHandleBytes[0x8];

    void* position =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x10 / 4])(object, nullptr);

    CSF_AT(std::uint32_t, positionBytes, 0) =
        CSF_AT(std::uint32_t, position, 0);
    CSF_AT(std::uint32_t, positionBytes, 4) =
        CSF_AT(std::uint32_t, position, 4);
    CSF_AT(std::uint32_t, positionBytes, 8) =
        CSF_AT(std::uint32_t, position, 8);

    void* attachment = CSF_AT(void*, self, 0x4);
    if (attachment &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attachment, 0)[0x44 / 4])(attachment, nullptr))
    {
        attachment = CSF_AT(void*, self, 0x4);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, attachment, 0)[0x3C / 4])(
                attachment, nullptr, object);
    }

    void* context = CSF_AT(void*, self, 0x8);
    if (context)
    {
        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0x14 / 4])(
                transform, nullptr, positionBytes);

        context = CSF_AT(void*, self, 0x8);
        transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, context, 0)[0x18 / 4])(context, nullptr);

        void* matrixHandle =
            reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, object, 0)[0x8C / 4])(
                    object, nullptr, matrixHandleBytes);

        void* matrixComponent =
            static_cast<std::uint8_t*>(CSF_AT(void*, matrixHandle, 0)) + 0x20;

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0x58 / 4])(
                transform, nullptr, matrixComponent);

        if (CSF_AT(std::uint8_t, matrixHandleBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHandleBytes, 0));
    }

    void* positionContext = CSF_AT(void*, self, 0xC);
    if (!positionContext)
        return 0;

    CSF_AT(float, positionBytes, 4) =
        duration * g_HalfDurationScale + CSF_AT(float, positionBytes, 4);

    return reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, positionContext, 0)[0x44 / 4])(
            positionContext, nullptr, positionBytes);
}

HOOK(0x006C8740, WeaponFirearms_UpdateContextFromObject, "auto");
