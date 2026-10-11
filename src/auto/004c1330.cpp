#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, TAiState_IsInParentChain, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_AiStateChain, 0x008B0550);

static int __fastcall TCombate_SerializeStateReferences(
    TCombate* self, void* /*edx*/, void* context)
{
    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    static constexpr std::size_t memberOffsets[] = {
        0x04, 0x24, 0x30, 0x48, 0x54, 0x78
    };

    uint8_t present;
    alignas(8) uint8_t referenceBytes[0x04];

    for (std::size_t offset : memberOffsets)
    {
        present = 0;
        if (CSF_AT(uint8_t, context, 0x14) != 0)
        {
            present = static_cast<uint8_t>(TAiState_IsInParentChain(
                g_AiStateChain, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + offset)));
        }

        const bool archiveFailed = CSF_AT(uint8_t, context, 0) != 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!archiveFailed)
        {
            void* archive = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            const int status =
                reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                    CSF_AT(void**, archive, 0)[0x44 / 4])(
                        archive, nullptr, &present, context);
            CSF_AT(uint8_t, context, 0) = status == -1;
        }

        if (present != 0)
        {
            const bool referenceFailed = CSF_AT(uint8_t, context, 0) != 0;
            CSF_AT(void*, referenceBytes, 0) =
                reinterpret_cast<uint8_t*>(self) + offset;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!referenceFailed)
            {
                CSF_AT(uint8_t, context, 0x15) = 1;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeObjectRef(
                    referenceBytes, reinterpret_cast<bool*>(context));
            }
        }
    }

    const bool archiveFailed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (!archiveFailed)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const int result =
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

HOOK(0x004C1330, TCombate_SerializeStateReferences, "auto");
