#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxCollisionShapes_AppendObjectOptions, 0x006CED70, (void* self, void* edx, uint32_t* a3));

static bool __stdcall GfxCollisionShapes_ReadObjectOptions(
    int* stream, S_006cec60* destination)
{
    using ReadFunction = int (__fastcall*)(void*, void*, void*, uint32_t);

    alignas(8) uint8_t textBytes[0x100];
    // The options record spans the adjacent locals in the original frame.
    alignas(8) uint8_t optionsBytes[0x64];

    auto* options = reinterpret_cast<uint32_t*>(optionsBytes);
    auto& firstString = *reinterpret_cast<C_0082d410*>(optionsBytes + 0x4);
    auto& secondString = *reinterpret_cast<C_0082d410*>(optionsBytes + 0x20);
    auto* text = reinterpret_cast<char*>(textBytes);

    int32_t recordCount = 0;
    uint32_t stringLength = 0;

    reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
        stream, nullptr, &recordCount, 4);

    for (int32_t recordIndex = 0; recordIndex < recordCount; ++recordIndex)
    {
        SceneMissionSetup_InitializeObjectOptions(options, nullptr);
        CSF_AT(uint32_t, options, 0x60) = 0;

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, options, 4) != 4)
            goto earlyReadFailure;

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, &stringLength, 4) != 4)
            goto earlyReadFailure;

        if (stringLength > 0)
        {
            int bytesRead =
                reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                    stream, nullptr, text, stringLength);
            if (static_cast<uint32_t>(bytesRead) != stringLength)
                goto readFailure;

            text[stringLength] = 0;
            AssignStringRange_429B60(
                &firstString, nullptr, text,
                static_cast<unsigned int>(std::strlen(text)));
        }

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, &stringLength, 4) != 4)
            goto readFailure;

        if (stringLength > 0)
        {
            int bytesRead =
                reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                    stream, nullptr, text, stringLength);
            if (static_cast<uint32_t>(bytesRead) != stringLength)
                goto readFailure;

            text[stringLength] = 0;
            AssignStringRange_429B60(
                &secondString, nullptr, text,
                static_cast<unsigned int>(std::strlen(text)));
        }

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, optionsBytes + 0x54, 4) != 4)
            goto readFailure;

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, optionsBytes + 0x58, 4) != 4)
            goto readFailure;

        if (reinterpret_cast<ReadFunction>(CSF_AT(void**, stream, 0)[8 / 4])(
                stream, nullptr, optionsBytes + 0x5C, 4) != 4)
            goto readFailure;

        GfxCollisionShapes_AppendObjectOptions(destination, nullptr, options);
        DestroySceneObjectOptionsStorage(options, nullptr);
    }

    return false;

earlyReadFailure:
    if (CSF_AT(uint32_t, &secondString, 0x18) >= 0x10 &&
        secondString.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(secondString.field_0004));
    }
    CSF_AT(uint32_t, &secondString, 0x18) = 15;
    CSF_AT(uint32_t, &secondString, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&secondString.field_0004)[0] = 0;

    if (CSF_AT(uint32_t, &firstString, 0x18) >= 0x10 &&
        firstString.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(firstString.field_0004));
    }
    return true;

readFailure:
    DestroySceneObjectOptionsStorage(options, nullptr);
    return true;
}

HOOK(0x006CF270, GfxCollisionShapes_ReadObjectOptions, "auto");
