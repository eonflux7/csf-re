#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CallHandlerIfNonNull_71B4C0, 0x0071B4C0,
         (void* context, void* edx, std::uint32_t destination,
          std::uint32_t* source));

CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300,
         (std::uint32_t* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// This embedded EH entry is not a standalone function start.
CSF_GLOBAL(std::uint8_t, g_RecordFillExceptionHandlerCode, 0x00827F50);
CSF_GLOBAL(std::uint8_t, g_RecordFillExceptionMetadata, 0x00891E04);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef volatile std::uint32_t
    __attribute__((address_space(257))) FsDword;

static __declspec(noinline) int __cdecl FillNonEmptyRecordRange(
    std::uint32_t destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    const std::uint32_t destinationBegin = destination;
    int result = 0;

    try
    {
        do
        {
            result = CallHandlerIfNonNull_71B4C0(
                context, nullptr, destination, source);

            --count;
            destination += 0x40u;
        }
        while (count != 0);
    }
    catch (...)
    {
        std::uint32_t completed = destinationBegin;
        while (completed != destination)
        {
            DestroyRecord_43E300(
                reinterpret_cast<std::uint32_t*>(completed), nullptr);
            completed += 0x40u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl FillRecordsWithHandler(
    std::uint32_t destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    if (count == 0)
    {
        // No EH frame in this wrapper: preserve the incoming FS:[0] result.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    return FillNonEmptyRecordRange(destination, count, source, context);
}

HOOK(0x0071B570, FillRecordsWithHandler, "auto");
