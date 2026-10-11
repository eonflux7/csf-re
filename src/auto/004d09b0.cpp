#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static bool __fastcall Scene_TestMatchingObjectsWithinThreshold(
    void* self, void* /*edx*/, void* collectionProvider, int typeFilter)
{
    bool matched = false;
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const float threshold = CSF_AT(float, settings, 0x1844);
    float* position = GetObjectPositionVector(
        CSF_AT(void*, self, 0x14), nullptr);

    alignas(8) uint8_t queryBytes[0x2C];
    CSF_AT(float, queryBytes, 0x00) = 0.0f;
    CSF_AT(float, queryBytes, 0x04) = 1.0f;
    CSF_AT(float, queryBytes, 0x08) = 0.0f;
    CSF_AT(uint32_t, queryBytes, 0x0C) = CSF_AT(uint32_t, position, 0x00);
    CSF_AT(uint32_t, queryBytes, 0x10) = CSF_AT(uint32_t, position, 0x04);
    CSF_AT(uint32_t, queryBytes, 0x14) = CSF_AT(uint32_t, position, 0x08);
    CSF_AT(float, queryBytes, 0x18) = 0.0f;
    CSF_AT(float, queryBytes, 0x1C) = 1.0f;
    CSF_AT(float, queryBytes, 0x20) = 0.0f;

    auto* query = reinterpret_cast<C_00833b88*>(queryBytes);
    NormalizeVector3(query, nullptr);

    void* entries =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, collectionProvider, 0)[0x38 / 4])(
                collectionProvider, nullptr);

    uint32_t index = 0;
    uint32_t byteOffset = 0;
    while (index < CSF_AT(uint32_t, entries, 0x08))
    {
        auto* entry = CSF_AT(uint8_t*, entries, 0x00) + byteOffset;
        if (typeFilter == -1 || CSF_AT(int, entry, 0x08) == typeFilter)
        {
            void* object = CSF_AT(void*, entry, 0x00);
            float measurement = 0.0f;
            if (reinterpret_cast<bool(__fastcall*)(
                    void*, void*, C_00833b88*, float*, int)>(
                    CSF_AT(void**, object, 0)[0x10 / 4])(
                        object, nullptr, query, &measurement, 0))
            {
                matched = measurement < threshold;
            }
        }

        ++index;
        byteOffset += 0x10;
        if (matched)
            break;
    }
    return matched;
}

HOOK(0x004D09B0, Scene_TestMatchingObjectsWithinThreshold, "auto");
