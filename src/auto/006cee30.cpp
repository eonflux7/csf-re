#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));
CSF_FUNC(bool, __fastcall, GfxCollisionShapes_AreShapesEqual, 0x006B2620, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, CollisionShape_CopyFrom, 0x006CE3A0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_006cec60_AppendElement100, 0x006CED70, (void* self, void* edx, uint32_t* a3));

static int __fastcall S_006cee30_ApplyWorldSetup(
    S_006cee30* self, void* /*edx*/, char* options)
{
    auto* entries = reinterpret_cast<S_006cec60*>(
        reinterpret_cast<uint8_t*>(self) + 4);

    uint32_t index = 0;
    uint32_t byteOffset = 0;
    int matchingIndex = -1;

    for (;;)
    {
        const uint32_t begin = entries->field_0004;
        if (!begin)
            break;

        const int count =
            static_cast<int32_t>(entries->field_0008 - begin) / 100;
        if (index >= static_cast<uint32_t>(count))
            break;

        if (GfxCollisionShapes_AreShapesEqual(options, nullptr,
                      static_cast<int32_t>(self->field_0008 + byteOffset)))
        {
            matchingIndex = static_cast<int32_t>(index);
        }

        ++index;
        byteOffset += 100;
    }

    if (matchingIndex != -1)
        return matchingIndex;

    alignas(8) uint8_t objectOptionsBytes[0x6C];
    SceneMissionSetup_InitializeObjectOptions(objectOptionsBytes, nullptr);
    // Last dword of the 100-byte element: the original clears it itself, between the two calls.
    CSF_AT(uint32_t, objectOptionsBytes, 0x60) = 0;
    CollisionShape_CopyFrom(objectOptionsBytes, nullptr,
              static_cast<int>(reinterpret_cast<intptr_t>(options)));
    S_006cec60_AppendElement100(entries, nullptr,
              reinterpret_cast<uint32_t*>(objectOptionsBytes));

    const uint32_t begin = entries->field_0004;
    const int count = begin
        ? static_cast<int32_t>(entries->field_0008 - begin) / 100
        : 0;
    const int appendedIndex =
        static_cast<int32_t>(static_cast<uint32_t>(count) - 1);

    if (CSF_AT(uint32_t, objectOptionsBytes, 0x38) >= 0x10)
    {
        void* block = CSF_AT(void*, objectOptionsBytes, 0x24);
        if (block)
            MemDebug_FreeDebugBlock(block);
    }

    const bool firstStringUsesHeap =
        CSF_AT(uint32_t, objectOptionsBytes, 0x1C) >= 0x10;
    CSF_AT(uint32_t, objectOptionsBytes, 0x38) = 15;
    CSF_AT(int, objectOptionsBytes, 0x34) = 0;
    CSF_AT(uint8_t, objectOptionsBytes, 0x24) = 0;

    if (firstStringUsesHeap)
    {
        void* block = CSF_AT(void*, objectOptionsBytes, 0x08);
        if (block)
            MemDebug_FreeDebugBlock(block);
    }

    return appendedIndex;
}

HOOK(0x006CEE30, S_006cee30_ApplyWorldSetup, "auto");

