#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static const uintptr_t g_ExceptionFrameInfo = CSF_ADDR_DATA(0x889AAC);

static uint32_t* __cdecl AiAttackTasks_CopyDwords(
    uint32_t* destination, int count, const uint32_t* source,
    int /*unusedA*/, int /*unusedB*/)
{
    uintptr_t cursor = reinterpret_cast<uintptr_t>(destination);
    uint32_t remaining = static_cast<uint32_t>(count);

    try
    {
        while (remaining != 0)
        {
            if (cursor != 0)
                *reinterpret_cast<uint32_t*>(cursor) = *source;

            --remaining;
            cursor += 4;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<uint32_t*>(cursor);
}

HOOK(0x0047EEC0, AiAttackTasks_CopyDwords, "auto");
