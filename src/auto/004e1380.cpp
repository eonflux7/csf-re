#include <cstdint>
#include "globals.h"
#include "hook.h"

// Runs up to three "child candidate" stages on the supplied context.
// Each stage asks the context's manager object to try to match one of three
// child slots taken from `self` (offsets 0x04, 0x10, 0x0C). A stage result of
// -1 marks the context as matched; the context's saved value (offset 0x1C) is
// restored afterwards and a final manager callback (vtable slot 0x7C) decides
// whether the context stays matched.
static int __fastcall ProcessContextChildStages(void* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(static_cast<std::uintptr_t>(contextAddress));

    bool stopped = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* object = CSF_AT(void*, context, 0x04);
        stopped = ((std::uint8_t(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, object, 0x00)[32])(object, nullptr, 0, context) == 0;
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0x00) = stopped;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!stopped)
    {
        void* object = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, object, 0x00)[22])(
                object, nullptr, static_cast<std::uint8_t*>(self) + 0x04, context);
        CSF_AT(std::uint8_t, context, 0x00) = result == -1;
    }

    bool processStage = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (processStage)
    {
        void* object = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, object, 0x00)[18])(
                object, nullptr, static_cast<std::uint8_t*>(self) + 0x10, context);
        CSF_AT(std::uint8_t, context, 0x00) = result == -1;
    }

    processStage = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (processStage)
    {
        void* object = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, object, 0x00)[22])(
                object, nullptr, static_cast<std::uint8_t*>(self) + 0x0C, context);
        CSF_AT(std::uint8_t, context, 0x00) = result == -1;
    }

    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(std::uint8_t, context, 0x00) != 0)
    {
        CSF_AT(std::uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* object = CSF_AT(void*, context, 0x04);
    const int result = ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, object, 0x00)[31])(object, nullptr, context);
    if (static_cast<std::uint8_t>(result) == 0)
    {
        CSF_AT(std::uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, context, 0x00) = 0;
    return result;
}

HOOK(0x004E1380, ProcessContextChildStages, "auto");
