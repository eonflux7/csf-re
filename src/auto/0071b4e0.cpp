#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CallHandlerIfNonNull, 0x0071B4C0,
         (void* context, void* edx, std::uint32_t destination,
          std::uint32_t* source));
CSF_FUNC(unsigned int, __fastcall, DestroyRecord, 0x0043E300,
         (std::uint32_t* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionHandlerCode, 0x00827F40);
CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionMetadata, 0x00891DB4);

static int __cdecl CopyRecordRangeWithHandler(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t destination,
    void* context)
{
    const std::uint32_t destinationBegin = destination;
    std::uint32_t source = reinterpret_cast<std::uint32_t>(first);

    try
    {
        while (source != reinterpret_cast<std::uint32_t>(last))
        {
            CallHandlerIfNonNull(
                context, nullptr, destination,
                reinterpret_cast<std::uint32_t*>(source));

            destination += 0x40u;
            source += 0x40u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = destinationBegin;
        while (completed != destination)
        {
            DestroyRecord(
                reinterpret_cast<std::uint32_t*>(completed), nullptr);
            completed += 0x40u;
        }

        RethrowCurrentException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(destination);
}

HOOK(0x0071B4E0, CopyRecordRangeWithHandler, "auto");
