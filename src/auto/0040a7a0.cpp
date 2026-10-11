#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, IndexedTransform_ScaleVector, 0x00401030,
         (void* self, void* edx, float* a2, float a3));
CSF_FUNC(int, __fastcall, ResolveIndexedEntry, 0x00406E90,
         (void* self, void* , int index));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateWorldX, 0x00407F60,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateWorldY, 0x00407FB0,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateWorldZ, 0x00408000,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_TranslateWorldX, 0x00408050,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(uint32_t*, __fastcall, IndexedTransform_TranslateWorldY, 0x004080C0,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(uint32_t*, __fastcall, IndexedTransform_TranslateWorldZ, 0x00408130,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateLocalX, 0x004081A0,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateLocalY, 0x00408200,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_RotateLocalZ, 0x00408270,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_TranslateLocalX, 0x004082E0,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_TranslateLocalY, 0x00408370,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_TranslateLocalZ, 0x00408400,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedTransform_ApplyExtraParameters, 0x00408490,
         (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));

CSF_GLOBAL(float, g_AxisX,   0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);
CSF_GLOBAL(float, g_AxisY,   0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_AxisZ,   0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x008A7D8C);

struct MatrixReference
{
    std::uint32_t* matrix;
    char owned;
    char padding[3];
};

static char __fastcall S_0040a7a0_ApplyTransform(
    S_0040a7a0* self, void* /*edx*/, int operation, char local, int valueBits)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    const int type =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[16 / 4])(owner, nullptr);

    float value;
    std::memcpy(&value, &valueBits, sizeof(value));

    switch (type)
    {
    case 0:
    {
        void* entry = reinterpret_cast<void*>(
            ResolveIndexedEntry(reinterpret_cast<void*>(self->field_0004),
                                nullptr, static_cast<int>(self->field_0008)));
        if (local)
        {
            int axisOffset;
            switch (operation)
            {
            case 0: case 3: axisOffset = 0; break;
            case 1: case 4: axisOffset = 16; break;
            case 2: case 5: axisOffset = 32; break;
            default: return static_cast<char>(operation);
            }

            MatrixReference matrixReference{};
            auto* returnedReference =
                reinterpret_cast<MatrixReference*(__fastcall*)(void*, void*, MatrixReference*)>(
                    CSF_AT(void**, entry, 0)[20 / 4])(
                        entry, nullptr, &matrixReference);

            if (operation < 3)
            {
                auto* axis = reinterpret_cast<float*>(
                    reinterpret_cast<std::uint8_t*>(returnedReference->matrix) + axisOffset);
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, int)>(
                    CSF_AT(void**, entry, 0)[44 / 4])(
                        entry, nullptr, axis, valueBits);
            }
            else
            {
                // The original captures this slot before constructing the displacement.
                auto translate = reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                    CSF_AT(void**, entry, 0)[40 / 4]);
                float displacement[3];
                auto* axis = reinterpret_cast<std::uint8_t*>(matrixReference.matrix) + axisOffset;
                float* scaled = IndexedTransform_ScaleVector(
                    axis, nullptr, displacement, value);
                translate(entry, nullptr, scaled);
            }

            const char owned = matrixReference.owned;
            if (owned)
                return static_cast<char>(RwMatrixDestroy(matrixReference.matrix));
            return owned;
        }

        switch (operation)
        {
        case 0:
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, entry, 0)[44 / 4])(entry, nullptr, &g_AxisX, valueBits);
        case 1:
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, entry, 0)[44 / 4])(entry, nullptr, &g_AxisY, valueBits);
        case 2:
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, entry, 0)[44 / 4])(entry, nullptr, &g_AxisZ, valueBits);
        case 3:
        {
            float displacement[3] = {
                g_AxisX * value, g_AxisX_Y * value, g_AxisX_Z * value
            };
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, entry, 0)[40 / 4])(entry, nullptr, displacement);
        }
        case 4:
        {
            float displacement[3] = {
                g_AxisY * value, g_AxisY_Y * value, g_AxisY_Z * value
            };
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, entry, 0)[40 / 4])(entry, nullptr, displacement);
        }
        case 5:
        {
            float displacement[3] = {
                g_AxisZ * value, g_AxisZ_Y * value, g_AxisZ_Z * value
            };
            return reinterpret_cast<char(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, entry, 0)[40 / 4])(entry, nullptr, displacement);
        }
        default:
            return static_cast<char>(operation);
        }
    }

    case 1: case 2: case 3: case 4: case 5: case 6: case 9: case 13:
        owner = reinterpret_cast<void*>(self->field_0004);
        if (local)
        {
            switch (operation)
            {
            case 0:
                return static_cast<char>(IndexedTransform_RotateLocalX(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 1:
                return static_cast<char>(IndexedTransform_RotateLocalY(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 2:
                return static_cast<char>(IndexedTransform_RotateLocalZ(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 3:
                return static_cast<char>(IndexedTransform_TranslateLocalX(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 4:
                return static_cast<char>(IndexedTransform_TranslateLocalY(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 5:
                return static_cast<char>(IndexedTransform_TranslateLocalZ(
                    owner, nullptr, static_cast<int>(self->field_0008), value));
            case 6:
                return static_cast<char>(IndexedTransform_ApplyExtraParameters(
                    owner, nullptr, static_cast<int>(self->field_0008), valueBits, 0));
            case 7:
                return static_cast<char>(IndexedTransform_ApplyExtraParameters(
                    owner, nullptr, static_cast<int>(self->field_0008), 0, valueBits));
            default:
                return static_cast<char>(operation);
            }
        }

        switch (operation)
        {
        case 0:
            return static_cast<char>(IndexedTransform_RotateWorldX(
                owner, nullptr, static_cast<int>(self->field_0008), value));
        case 1:
            return static_cast<char>(IndexedTransform_RotateWorldY(
                owner, nullptr, static_cast<int>(self->field_0008), value));
        case 2:
            return static_cast<char>(IndexedTransform_RotateWorldZ(
                owner, nullptr, static_cast<int>(self->field_0008), value));
        case 3:
            return static_cast<char>(IndexedTransform_TranslateWorldX(
                owner, nullptr, static_cast<int>(self->field_0008), value));
        case 4:
            return static_cast<char>(reinterpret_cast<std::uintptr_t>(
                IndexedTransform_TranslateWorldY(
                    owner, nullptr, static_cast<int>(self->field_0008), value)));
        case 5:
            return static_cast<char>(reinterpret_cast<std::uintptr_t>(
                IndexedTransform_TranslateWorldZ(
                    owner, nullptr, static_cast<int>(self->field_0008), value)));
        case 6:
            return static_cast<char>(IndexedTransform_ApplyExtraParameters(
                owner, nullptr, static_cast<int>(self->field_0008), valueBits, 0));
        case 7:
            return static_cast<char>(IndexedTransform_ApplyExtraParameters(
                owner, nullptr, static_cast<int>(self->field_0008), 0, valueBits));
        default:
            return static_cast<char>(operation);
        }

    default:
        return static_cast<char>(type);
    }
}

HOOK(0x0040A7A0, S_0040a7a0_ApplyTransform, "auto");
