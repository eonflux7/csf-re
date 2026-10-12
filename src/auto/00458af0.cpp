#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall HudStatusIcons_ProcessState(
    void* self, void* /*edx*/, void* context)
{
    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);

    bool stopped = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 4);
        stopped = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, processor, 0)[32])(
                processor, nullptr, 0, context);
    }

    CSF_AT(std::uint8_t, context, 0) = stopped;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!stopped)
    {
        void* processor = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[22])(
                    processor, nullptr, self, context) == -1;
    }

    const std::uint32_t memberOffsets[] = {8, 12, 4, 16, 20, 24};
    for (std::uint32_t offset : memberOffsets)
    {
        const bool shouldProcess = CSF_AT(std::uint8_t, context, 0) == 0;
        CSF_AT(std::uint32_t, context, 0x1C) = 0;
        if (shouldProcess)
        {
            void* processor = CSF_AT(void*, context, 4);
            CSF_AT(std::uint8_t, context, 0x15) = 0;
            CSF_AT(std::uint8_t, context, 0x16) = 0;
            CSF_AT(std::uint8_t, context, 0) =
                ((int(__fastcall*)(void*, void*, void*, void*))
                    CSF_AT(void**, processor, 0)[21])(
                        processor, nullptr,
                        static_cast<std::uint8_t*>(self) + offset,
                        context) == -1;
        }
    }

    const bool shouldProcessLast = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (shouldProcessLast)
    {
        void* processor = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[14])(
                    processor, nullptr,
                    static_cast<std::uint8_t*>(self) + 28,
                    context) == -1;
    }

    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(std::uint8_t, context, 0) != 0)
    {
        CSF_AT(std::uint8_t, context, 0) = 1;
        return 1;
    }

    void* processor = CSF_AT(void*, context, 4);
    const bool result =
        ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, processor, 0)[31])(
                processor, nullptr, context);

    if (!result)
    {
        CSF_AT(std::uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x00458AF0, HudStatusIcons_ProcessState, "auto");
