#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(char, __fastcall, ObjectProvider_TryFindIndexedEntry, 0x004F6E90, (void* self, void* edx, int index, uint32_t* outIndex));
CSF_FUNC(uint32_t*, __fastcall, CharacterModel_InsertProviderSlotBlend, 0x004F8440, (void* self, void* edx, uint32_t* a2, int a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);

static char __fastcall S_004f6e90_ApplyProviderSlotBlend(
    S_004f6e90* self, void* /*edx*/,
    int slotIndex, int sourceHandle, int targetHandle, int blendFlags)
{
    alignas(8) uint8_t entryBytes[0x40];
    auto* entry = reinterpret_cast<uint32_t*>(entryBytes);

    // The packet additionally tags this direct-call address as a vtable.
    (void)CSF_VTBL(0x004F6E90);

    CSF_AT(int, entryBytes, 0x00) = -1;
    CSF_AT(int, entryBytes, 0x08) = -1;
    CSF_AT(int, entryBytes, 0x0C) = -1;
    CSF_AT(int, entryBytes, 0x14) = -1;
    CSF_AT(int, entryBytes, 0x18) = -1;
    CSF_AT(void**, entryBytes, 0x04) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, entryBytes, 0x10) = CSF_VTBL(0x0082D448);
    CSF_AT(int, entryBytes, 0x1C) = 0;
    CSF_AT(float, entryBytes, 0x20) = 0.0f;
    CSF_AT(float, entryBytes, 0x24) = 0.0f;
    CSF_AT(float, entryBytes, 0x28) = 0.0f;
    CSF_AT(float, entryBytes, 0x2C) = 0.0f;

    char result = ObjectProvider_TryFindIndexedEntry(
        self, nullptr, slotIndex, entry);
    if (result)
        return result;

    void* source = reinterpret_cast<void*>(sourceHandle);
    void* target = reinterpret_cast<void*>(targetHandle);

    int sourceId = CSF_AT(int, source, 0x04);
    int sourceIndex = CSF_AT(int, source, 0x08);
    CSF_AT(int, entryBytes, 0x08) = sourceId;
    CSF_AT(int, entryBytes, 0x00) = slotIndex;

    int targetId = CSF_AT(int, target, 0x04);
    int targetIndex = CSF_AT(int, target, 0x08);
    CSF_AT(int, entryBytes, 0x18) = targetIndex;
    CSF_AT(int, entryBytes, 0x0C) = sourceIndex;
    CSF_AT(int, entryBytes, 0x14) = targetId;
    CSF_AT(int, entryBytes, 0x1C) = blendFlags;

    if (sourceIndex >= 0)
    {
        auto* object = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, sourceHandle));
        if (object)
        {
            void* provider = reinterpret_cast<void*>(self->field_0004);
            void* positionObject =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, provider, 0)[0x10 / 4])(
                        provider, nullptr);

            float* objectPosition = reinterpret_cast<float*>(
                GetObjectPositionVector(object, nullptr));
            float* providerPosition =
                ((float* (__fastcall*)(void*, void*))
                    CSF_AT(void**, positionObject, 0)[0x10 / 4])(
                        positionObject, nullptr);

            float deltaX = providerPosition[0] - objectPosition[0];
            float deltaY = providerPosition[1] - objectPosition[1];
            float deltaZ = providerPosition[2] - objectPosition[2];
            CSF_AT(float, entryBytes, 0x24) = deltaY;
            CSF_AT(float, entryBytes, 0x20) = deltaX;
            CSF_AT(float, entryBytes, 0x28) = deltaZ;

            double scalar =
                C_008342e8_ComputeResolvedObjectScalar(object, nullptr);
            CSF_AT(float, entryBytes, 0x2C) =
                static_cast<float>(scalar * g_DegreesToRadians);
        }
    }

    uint32_t* inserted = CharacterModel_InsertProviderSlotBlend(
        reinterpret_cast<uint8_t*>(self) + 0x11C, nullptr, entry, 0);
    return static_cast<char>(reinterpret_cast<uintptr_t>(inserted));
}

HOOK(0x004F8800, S_004f6e90_ApplyProviderSlotBlend, "auto");
