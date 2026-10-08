#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00865320);

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, TranslateMatrixHandle, 0x006ADBB0,
         (void* self, void* edx, float x, float y, float z));
CSF_FUNC(int, __fastcall, RotateMatrixHandle, 0x006AE210,
         (void* self, void* edx, float radians));

struct RotationMatrixHandle
{
    std::uint32_t* matrix;
    std::uint8_t owned;
};

static void __cdecl ApplyCenteredRotation(void* object, float degrees)
{
    if (!object)
        return;

    auto* position =
        ((std::int32_t* (__fastcall*)(void*, void*))
         CSF_AT(void**, object, 0)[11])(object, nullptr);
    auto* size =
        ((std::int32_t* (__fastcall*)(void*, void*))
         CSF_AT(void**, object, 0)[15])(object, nullptr);

    const float centerX = static_cast<float>(
        static_cast<double>(size[0]) *
        static_cast<double>(g_HalfDurationScale) +
        static_cast<double>(position[0]));
    const float centerY = static_cast<float>(
        static_cast<double>(size[1]) *
        static_cast<double>(g_HalfDurationScale) +
        static_cast<double>(position[1]));

    RotationMatrixHandle handle;
    handle.matrix = RwMatrixCreate();
    handle.owned = 1;

    CSF_AT(std::uint32_t, handle.matrix, 0x28) = 0x3F800000;
    CSF_AT(std::uint32_t, handle.matrix, 0x14) = 0x3F800000;
    CSF_AT(std::uint32_t, handle.matrix, 0x00) = 0x3F800000;
    CSF_AT(std::uint32_t, handle.matrix, 0x10) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x08) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x04) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x24) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x20) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x18) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x38) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x34) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x30) = 0;
    CSF_AT(std::uint32_t, handle.matrix, 0x0C) |= 0x20003;

    TranslateMatrixHandle(&handle, nullptr, -centerX, -centerY, 0.0f);

    const float radians = static_cast<float>(
        static_cast<double>(degrees) *
        static_cast<double>(g_DegreesToRadians));
    RotateMatrixHandle(&handle, nullptr, radians);

    TranslateMatrixHandle(&handle, nullptr, centerX, centerY, 0.0f);

    ((void (__fastcall*)(void*, void*, RotationMatrixHandle*))
     CSF_AT(void**, object, 0)[24])(object, nullptr, &handle);

    if (handle.owned)
        RwMatrixDestroy(handle.matrix);
}

HOOK(0x00725630, ApplyCenteredRotation, "auto");
