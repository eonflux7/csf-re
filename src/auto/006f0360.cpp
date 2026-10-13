#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, DynamicObjects_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, AiAttackTasks_UpdateSlotFromManager, 0x006B32C0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Retained as explicitly listed in the packet.
static const auto kStageHandlersVtable = CSF_VTBL(0x006CA6F0);

static int __fastcall S_006ca6f0_SerializeParticles(
    S_006ca6f0* self, void* /*edx*/, unsigned int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    bool initialFlag = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        initialFlag =
            !reinterpret_cast<bool (__fastcall*)(void*, void*, int, unsigned int)>(
                CSF_AT(void**, handler, 0)[0x80 / 4])(
                    handler, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = initialFlag;
    ProcessContextStageHandlers(self, nullptr, static_cast<int>(contextAddress));

    bool active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        AiAttackTasks_UpdateSlotFromManager(
            static_cast<int>(reinterpret_cast<uintptr_t>(
                reinterpret_cast<uint8_t*>(self) + 0x1C)),
            reinterpret_cast<S_00691860*>(context));
    }

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& particleCount = *reinterpret_cast<uint32_t*>(countBytes);
    particleCount = 0; // The original leaves this uninitialized on some paths.
    if (CSF_AT(uint8_t, context, 0x14) != 0)
        particleCount = self->field_00ac;

    active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (active)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int status =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, unsigned int)>(
                CSF_AT(void**, handler, 0)[0x48 / 4])(
                    handler, nullptr, &particleCount, contextAddress);
        CSF_AT(uint8_t, context, 0) = status == -1;
    }

    alignas(8) uint8_t referenceBytes[0x4];
    int& reference = *reinterpret_cast<int*>(referenceBytes);

    unsigned int index = 0;
    if (particleCount != 0)
    {
        auto* particles = reinterpret_cast<uint8_t*>(self) + 0x18;
        do
        {
            const bool indexed = CSF_AT(uint8_t, context, 0x14) != 0;
            void** vtable = CSF_AT(void**, particles, 0);
            if (indexed)
            {
                reference =
                    reinterpret_cast<int (__fastcall*)(void*, void*, unsigned int)>(
                        vtable[0x18 / 4])(particles, nullptr, index);
            }
            else
            {
                reference =
                    reinterpret_cast<int (__fastcall*)(void*, void*, float*)>(
                        vtable[0xC / 4])(particles, nullptr, &g_ZeroVectorX);
            }

            active = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (active)
            {
                CSF_AT(uint8_t, context, 0x15) = 1;
                CSF_AT(uint8_t, context, 0x16) = 0;
                DynamicObjects_SerializeReference(
                    referenceBytes, reinterpret_cast<bool*>(context));
            }
            ++index;
        } while (index < particleCount);
    }

    active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (active)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, unsigned int)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(
                    handler, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x006F0360, S_006ca6f0_SerializeParticles, "auto");
