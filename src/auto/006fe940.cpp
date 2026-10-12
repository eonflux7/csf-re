#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GrowRecordArray, 0x006D9EB0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall GfxClumpLoader_AddObject(void* self, void* /*edx*/, void* object)
{
    alignas(8) uint8_t halfSizeBytes[0xC];
    alignas(8) uint8_t sizeBytes[0xC];

    CSF_AT(float, halfSizeBytes, 0x4) = 0.0f;

    if (CSF_AT(uint32_t, self, 0x2C) >= CSF_AT(uint32_t, self, 0x28))
    {
        GrowRecordArray(
            static_cast<uint8_t*>(self) + 0x24,
            nullptr,
            CSF_AT(int, self, 0x30));
    }

    const uint32_t index = CSF_AT(uint32_t, self, 0x2C);
    uint8_t* record = CSF_AT(uint8_t*, self, 0x24) + 12 * index;
    CSF_AT(uint32_t, self, 0x2C) = index + 1;
    CSF_AT(void*, record, 0x0) = object;
    CSF_AT(float, record, 0x4) = CSF_AT(float, halfSizeBytes, 0x4);
    CSF_AT(float, record, 0x8) = 0.0f;

    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, object, 0)[0x50 / 4])(
            object, nullptr, reinterpret_cast<float*>(sizeBytes));

    CSF_AT(float, halfSizeBytes, 0x0) =
        CSF_AT(float, sizeBytes, 0x0) * g_HalfDurationScale;
    CSF_AT(float, halfSizeBytes, 0x4) =
        CSF_AT(float, sizeBytes, 0x4) * g_HalfDurationScale;
    CSF_AT(float, halfSizeBytes, 0x8) =
        CSF_AT(float, sizeBytes, 0x8) * g_HalfDurationScale;

    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, object, 0)[0x4C / 4])(
            object, nullptr, reinterpret_cast<float*>(halfSizeBytes));

    CSF_AT(uint32_t, CSF_AT(void*, object, 0x4), 0x8) &= ~1u;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* renderHandle =
        reinterpret_cast<void*>(RenderHandle_FromObject(globalObject, nullptr));
    void* registry = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, renderHandle, 0)[0x38 / 4])(renderHandle, nullptr);

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, registry, 0)[0])(registry, nullptr, object);

    return static_cast<int>(CSF_AT(uint32_t, self, 0x2C) - 1);
}

HOOK(0x006FE940, GfxClumpLoader_AddObject, "auto");
