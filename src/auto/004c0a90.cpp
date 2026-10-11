#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Archive_IsObjectReferenceRegistered, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceRegistry, 0x008B0550);

static int __fastcall AiStateFactory_SerializeObjectReferences(
    char* self, void* /*edx*/, void* context)
{
    const uint32_t savedArchiveState = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;

    AudioSoundManager_ProcessArchiveContext(context);

    alignas(8) uint8_t referenceBytes[0x4];
    const int referenceOffsets[] = {4, 8, 16};

    for (int offset : referenceOffsets)
    {
        uint8_t referencePresent = 0;
        if (CSF_AT(uint8_t, context, 0x14) != 0)
        {
            referencePresent = static_cast<uint8_t>(
                Archive_IsObjectReferenceRegistered(
                    g_ObjectReferenceRegistry, nullptr,
                    reinterpret_cast<int>(self + offset)));
        }

        const bool canSerializePresence = CSF_AT(uint8_t, context, 0) == 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (canSerializePresence)
        {
            void* archive = CSF_AT(void*, context, 4);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            const int result =
                reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                    CSF_AT(void**, archive, 0)[0x44 / 4])(
                        archive, nullptr, &referencePresent, context);
            CSF_AT(uint8_t, context, 0) = result == -1;
        }

        if (referencePresent != 0)
        {
            const bool canSerializeReference = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(char*, referenceBytes, 0) = self + offset;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerializeReference)
            {
                CSF_AT(uint8_t, context, 0x15) = 1;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeObjectRef(
                    referenceBytes, reinterpret_cast<bool*>(context));
            }
        }
    }

    const bool canFinish = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedArchiveState;
    if (canFinish)
    {
        void* archive = CSF_AT(void*, context, 4);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, context);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004C0A90, AiStateFactory_SerializeObjectReferences, "auto");
