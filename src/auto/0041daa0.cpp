#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4084C0, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_4087D0, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(float*, __fastcall, sub_40B550, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __fastcall, sub_41D640, 0x0041D640, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

static int __fastcall C_0082e428_InitializeSteeringAlignment(
    C_0082e428* self, void* /*edx*/)
{
    struct MatrixResult
    {
        std::uint32_t* matrix;
        char owned;
        char padding[3];
    };

    void* part = sub_4087D0(
        reinterpret_cast<void*>(self->field_0030), nullptr, 25);

    void* entry = reinterpret_cast<void*>(sub_4084C0(
        reinterpret_cast<void*>(self->field_0030), nullptr, 25));

    MatrixResult matrixStorage{};
    MatrixResult* matrixResult =
        reinterpret_cast<MatrixResult* (__fastcall*)(
            void*, void*, MatrixResult*)>(
                CSF_AT(void**, entry, 0)[0x40 / 4])(
                    entry, nullptr, &matrixStorage);

    S_00526d40 direction;
    std::memcpy(
        &direction,
        reinterpret_cast<std::uint8_t*>(matrixResult->matrix) + 0x20,
        0xC);

    if (matrixStorage.owned)
        RwMatrixDestroy(matrixStorage.matrix);

    float alignment[4];
    alignment[0] = static_cast<float>(
        -S_00526d40_GetVectorAngleRadians(&direction, nullptr));
    alignment[1] = 0.2f;
    alignment[2] = 0.0f;
    alignment[3] = -1.0f;

    sub_40B550(part, nullptr, 1, alignment);
    int result = sub_41D640(self, nullptr, 1);
    self->field_008c = 4;
    return result;
}

HOOK(0x0041DAA0, C_0082e428_InitializeSteeringAlignment, "auto");
