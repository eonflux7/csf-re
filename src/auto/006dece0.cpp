#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, CopyConstructTextureRecord, 0x006DECC0,
         (void* a1, int a2));
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220,
         (int* record));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_TextureCopyRangeFuncInfo, 0x00890D84);

static int __cdecl GfxTextureLoader_CopyConstructRecordRange(
    int sourceBegin, int sourceEnd, int destination,
    int /*context*/, int /*unusedArg5*/)
{
    const int destinationBegin = destination;
    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyConstructTextureRecord(
                reinterpret_cast<void*>(destination), sourceBegin);
            destination = static_cast<int>(
                static_cast<std::uint32_t>(destination) + 16);
            sourceBegin = static_cast<int>(
                static_cast<std::uint32_t>(sourceBegin) + 16);
        }
    }
    catch (...)
    {
        int current = destinationBegin;
        while (current != destination)
        {
            DestroyElementThroughContext(reinterpret_cast<int*>(current));
            current = static_cast<int>(
                static_cast<std::uint32_t>(current) + 16);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }
    return destination;
}

HOOK(0x006DECE0, GfxTextureLoader_CopyConstructRecordRange, "auto");
