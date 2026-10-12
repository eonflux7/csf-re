#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, AiStatic_SetSegmentEndpoints, 0x006AB460, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxCollisionShapes_AppendObjectOptions, 0x006CED70, (void* self, void* edx, uint32_t* a3));

static int GfxCollisionShapes_ReadBytes(int* stream, void* destination, uint32_t size)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*, void*, uint32_t)>(
        CSF_AT(void**, stream, 0)[8 / 4])(stream, nullptr, destination, size);
}

static bool __stdcall GfxCollisionShapes_ReadObjectOptions(
    int* stream, S_006cec60* destination)
{
    char text[256];
    // The options record spans several separately identified stack locals.
    // Reserve the original frame size to accommodate the complete object.
    alignas(8) uint8_t optionsBytes[0x184];
    auto& firstString = *reinterpret_cast<C_0082d410*>(optionsBytes + 4);
    auto& secondString = *reinterpret_cast<C_0082d410*>(optionsBytes + 0x20);

    int32_t count = 0;
    uint32_t textLength = 0;
    bool failed = false;
    GfxCollisionShapes_ReadBytes(stream, &count, 4);

    if (count <= 0)
        return failed;

    uint32_t index = 0;
    do
    {
        SceneMissionSetup_InitializeObjectOptions(optionsBytes, nullptr);
        CSF_AT(uint32_t, optionsBytes, 0x60) = 0;

        failed = GfxCollisionShapes_ReadBytes(stream, optionsBytes, 4) != 4;
        if (!failed)
            failed = GfxCollisionShapes_ReadBytes(stream, &textLength, 4) != 4;

        if (failed)
        {
            // These two early failures only destroy the string members.
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
            return failed;
        }

        uint32_t firstEndpoint[3];
        uint32_t secondEndpoint[3];

        do
        {
            if (textLength)
            {
                int bytesRead = GfxCollisionShapes_ReadBytes(stream, text, textLength);
                failed = static_cast<uint32_t>(bytesRead) != textLength;
                if (failed)
                    break;
                text[textLength] = 0;
                AssignStringRange_429B60(
                    &firstString, nullptr, text,
                    static_cast<unsigned int>(std::strlen(text)));
            }

            failed = GfxCollisionShapes_ReadBytes(stream, &textLength, 4) != 4;
            if (failed)
                break;

            if (textLength)
            {
                int bytesRead = GfxCollisionShapes_ReadBytes(stream, text, textLength);
                failed = static_cast<uint32_t>(bytesRead) != textLength;
                if (failed)
                    break;
                text[textLength] = 0;
                AssignStringRange_429B60(
                    &secondString, nullptr, text,
                    static_cast<unsigned int>(std::strlen(text)));
            }

            secondEndpoint[0] = 0;
            secondEndpoint[1] = 0;
            secondEndpoint[2] = 0;
            firstEndpoint[0] = 0;
            firstEndpoint[1] = 0;
            firstEndpoint[2] = 0;

            failed = GfxCollisionShapes_ReadBytes(stream, &secondEndpoint[0], 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, &secondEndpoint[1], 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, &secondEndpoint[2], 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, &firstEndpoint[0], 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, &firstEndpoint[1], 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, &firstEndpoint[2], 4) != 4;
            if (failed)
                break;

            AiStatic_SetSegmentEndpoints(
                optionsBytes + 0x3C, nullptr, firstEndpoint, secondEndpoint);

            failed = GfxCollisionShapes_ReadBytes(stream, optionsBytes + 0x54, 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, optionsBytes + 0x58, 4) != 4;
            if (failed)
                break;
            failed = GfxCollisionShapes_ReadBytes(stream, optionsBytes + 0x5C, 4) != 4;
            if (failed)
                break;

            GfxCollisionShapes_AppendObjectOptions(
                destination, nullptr, reinterpret_cast<uint32_t*>(optionsBytes));
        } while (false);

        DestroySceneObjectOptionsStorage(optionsBytes, nullptr);
        if (failed)
            return failed;

        ++index;
    } while (static_cast<int32_t>(index) < count);

    return failed;
}

HOOK(0x006CEF70, GfxCollisionShapes_ReadObjectOptions, "auto");
