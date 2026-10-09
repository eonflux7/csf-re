#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall AudioSoundManager_SerializeObjectReferences(
    void* self, void* /*edx*/, void* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }

    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            static_cast<uint8_t*>(self) + 8,
            static_cast<bool*>(archive));
    }

    static constexpr uint32_t referenceOffsets[] = {
        12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52
    };

    for (uint32_t offset : referenceOffsets)
    {
        const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
        CSF_AT(uint32_t, archive, 0x1C) = 0;
        if (canSerialize)
        {
            CSF_AT(uint8_t, archive, 0x15) = 0;
            CSF_AT(uint8_t, archive, 0x16) = 0;
            Archive_SerializeObjectRef(
                static_cast<uint8_t*>(self) + offset,
                static_cast<bool*>(archive));
        }
    }

    const bool canFinish = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canFinish)
    {
        void* backend = CSF_AT(void*, archive, 4);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archive);
        if (result)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00526020, AudioSoundManager_SerializeObjectReferences, "auto");
