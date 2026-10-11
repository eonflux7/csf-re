#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowBindingArray, 0x004085C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static char __fastcall C_0082d5e0_ResolvePendingBindings(
    C_0082d5e0* self, void* /*edx*/, int* source)
{
    self->field_0008 = reinterpret_cast<std::uint32_t>(source);

    void* bindingProvider =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x28 / 4])(source, nullptr);

    void* matrixProvider =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, bindingProvider, 0)[0x68 / 4])(
                bindingProvider, nullptr);

    const std::uint32_t pendingCount = self->field_0034;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x10);
    if (pendingCount > capacity)
    {
        GrowBindingArray(
            reinterpret_cast<std::uint8_t*>(self) + 0x0C,
            nullptr, static_cast<int>(pendingCount - capacity));
    }
    self->field_0014 = pendingCount;

    std::uint32_t index = 0;
    std::uint32_t pendingOffset = 0;
    std::uint32_t bindingOffset = 0;
    if (self->field_0034 != 0)
    {
        do
        {
            auto* pending = reinterpret_cast<std::uint8_t*>(self->field_002c)
                + pendingOffset;
            const int bindingIndex =
                ((int (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, bindingProvider, 0)[0x88 / 4])(
                        bindingProvider, nullptr, CSF_AT(int, pending, 0x04));

            if (bindingIndex == -1)
            {
                auto* binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                    + bindingOffset;
                CSF_AT(int, binding, 0) = 0;
            }
            else
            {
                auto* binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                    + bindingOffset;
                const int resolvedBinding =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, bindingProvider, 0)[0x80 / 4])(
                            bindingProvider, nullptr, bindingIndex);
                CSF_AT(int, binding, 0) = resolvedBinding;
            }

            auto* binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                + bindingOffset;
            if (matrixProvider)
            {
                pending = reinterpret_cast<std::uint8_t*>(self->field_002c)
                    + pendingOffset;
                const int matrixIndex =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, matrixProvider, 0)[0x68 / 4])(
                            matrixProvider, nullptr, CSF_AT(int, pending, 0x08));
                CSF_AT(int, binding, 0x04) = matrixIndex;

                binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                    + bindingOffset;
                const int resolvedMatrixIndex = CSF_AT(int, binding, 0x04);
                if (resolvedMatrixIndex != -1)
                {
                    std::uint32_t matrixResult[7]{};
                    self->vSlot28(
                        reinterpret_cast<int>(matrixResult), resolvedMatrixIndex);

                    binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                        + bindingOffset;
                    std::memcpy(
                        CSF_AT(void*, binding, 0x08),
                        reinterpret_cast<void*>(matrixResult[0]), 0x40u);

                    binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                        + bindingOffset;
                    GfxMatrix_CopyTranslationAndUpdate(
                        binding + 0x08, nullptr,
                        reinterpret_cast<const uint32_t*>(&g_ZeroVectorX));

                    auto* matrix = reinterpret_cast<std::uint8_t*>(matrixResult[0]);
                    binding = reinterpret_cast<std::uint8_t*>(self->field_000c)
                        + bindingOffset;
                    CSF_AT(std::uint32_t, binding, 0x10) =
                        CSF_AT(std::uint32_t, matrix, 0x30);
                    CSF_AT(std::uint32_t, binding, 0x14) =
                        CSF_AT(std::uint32_t, matrix, 0x34);
                    CSF_AT(std::uint32_t, binding, 0x18) =
                        CSF_AT(std::uint32_t, matrix, 0x38);

                    if (CSF_AT(std::uint8_t, matrixResult, 0x04))
                        RwMatrixDestroy(
                            reinterpret_cast<std::uint32_t*>(matrixResult[0]));
                }
            }
            else
            {
                CSF_AT(int, binding, 0x04) = -1;
            }

            ++index;
            pendingOffset += 0x0C;
            bindingOffset += 0x1C;
        } while (index < self->field_0034);
    }

    void* pendingStorage = reinterpret_cast<void*>(self->field_002c);
    self->field_0030 = 0;
    self->field_0034 = 0;
    if (pendingStorage)
    {
        Mem_Free(pendingStorage);
        self->field_002c = 0;
    }
    return 1;
}

HOOK(0x004088B0, C_0082d5e0_ResolvePendingBindings, "auto");
