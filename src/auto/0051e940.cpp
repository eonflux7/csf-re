#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00840f48_CreateParticle, 0x0051E440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00840f48_FindParticleEntry, 0x0051E550, (C_00840f48* self, void* , int particle));
CSF_FUNC(uint32_t*, __fastcall, FxParticleManager_GrowEntryArray, 0x0051E720, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_00840f48_HandleParticleEvent(
    C_00840f48* self, void* /*edx*/, std::uint32_t* event)
{
    if (event[1] == 19)
    {
        const std::uint32_t index = static_cast<std::uint32_t>(
            C_00840f48_FindParticleEntry(
                self, nullptr,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(event + 2))));

        if (index != static_cast<std::uint32_t>(-1))
        {
            const std::uint32_t entryOffset = 16 * index;
            void* particle = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_000c + entryOffset), 0);

            if (particle)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, particle, 0)[44 / 4])(particle, nullptr, 0);

                void* currentParticle = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_000c + entryOffset), 0);
                void** currentVtable = CSF_AT(void**, currentParticle, 0);
                void** originalVtable = CSF_AT(void**, particle, 0);

                alignas(8) std::uint8_t matrixResultBytes[0x10];
                const int matrixResult =
                    ((int(__fastcall*)(void*, void*, void*))
                        currentVtable[76 / 4])(
                            currentParticle, nullptr, matrixResultBytes);

                ((void(__fastcall*)(void*, void*, int))
                    originalVtable[64 / 4])(particle, nullptr, matrixResult);

                if (CSF_AT(std::uint8_t, matrixResultBytes, 4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

                auto* entry = reinterpret_cast<std::uint32_t*>(
                    self->field_000c + entryOffset);

                if (self->field_0024 >= self->field_0020)
                {
                    (void)CSF_VTBL(0x0041E5F0);
                    S_0041e5f0_GrowArrayCapacity(
                        reinterpret_cast<S_0041e5f0*>(
                            reinterpret_cast<std::uint8_t*>(self) + 0x1C),
                        nullptr, static_cast<int>(CSF_AT(std::uint32_t, self, 0x28)));
                }

                const std::uint32_t retiredCount = self->field_0024;
                auto* retiredSlot = reinterpret_cast<std::uint32_t*>(
                    self->field_001c + 4 * retiredCount);
                self->field_0024 = retiredCount + 1;
                *retiredSlot = *entry;

                CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(self->field_000c + entryOffset), 0) = 0;
            }

            if (index < self->field_0014 - 1)
            {
                auto* lastEntry = reinterpret_cast<std::uint32_t*>(
                    self->field_000c + 16 * self->field_0014 - 16);
                const std::uint32_t lastParticle = lastEntry[0];
                auto* destination = reinterpret_cast<std::uint32_t*>(
                    self->field_000c + entryOffset);

                destination[0] = lastParticle;
                destination[2] = lastEntry[2];
                destination[3] = lastEntry[3];
            }

            --self->field_0014;
        }
    }

    if (event[1] == 42)
    {
        const int particleHandle =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(event + 2));

        CHandleTable_Resolve(g_pCurrentScene, nullptr, particleHandle);
        int index = C_00840f48_FindParticleEntry(self, nullptr, particleHandle);

        if (index == -1)
        {
            const std::uint32_t handleHigh = event[4];
            const std::uint32_t handleLow = event[3];

            if (self->field_0014 >= CSF_AT(std::uint32_t, self, 0x10))
            {
                FxParticleManager_GrowEntryArray(
                    reinterpret_cast<std::uint8_t*>(self) + 0x0C,
                    nullptr, static_cast<int>(CSF_AT(std::uint32_t, self, 0x18)));
            }

            const std::uint32_t entryCount = self->field_0014;
            auto* entry = reinterpret_cast<std::uint32_t*>(
                self->field_000c + 16 * entryCount);
            self->field_0014 = entryCount + 1;
            entry[0] = 0;
            entry[2] = handleLow;
            entry[3] = handleHigh;
            index = static_cast<int>(self->field_0014 - 1);
        }

        const std::uint32_t entryOffset = 16 * static_cast<std::uint32_t>(index);
        if (!CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(self->field_000c + entryOffset), 0))
        {
            const int particle =
                C_00840f48_CreateParticle(self, nullptr, particleHandle);
            if (particle)
            {
                CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(self->field_000c + entryOffset), 0) =
                    static_cast<std::uint32_t>(particle);
            }
        }
    }
}

HOOK(0x0051E940, C_00840f48_HandleParticleEvent, "auto");
