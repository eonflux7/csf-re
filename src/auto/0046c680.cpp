#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies two dwords from src to dest when dest is non-null (stdcall; ecx is not consumed).
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

// Releases one already-processed 8-byte record (stdcall; ecx is not consumed).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecord, 0x0046C460,
         (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Unwind/frame info referenced by the exception-frame prologue.
[[maybe_unused]] static const void* const kMissionScriptUnwindInfo =
    reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00888EDC));

// Copies a run of `count` 8-byte records starting at `firstRecord`, using `srcArg` as the
// source for each copy. If a copy throws, the records already processed are released
// again and the exception is rethrown.
static int __cdecl MissionScript_CopyRecordsRange(
    int firstRecord,
    std::uint32_t count,
    int srcArg,
    int /*recordContext*/,
    int /*unused*/)
{
    int currentRecord = firstRecord;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyTwoDwordsIfDestNonNull(
                    reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(currentRecord)),
                    reinterpret_cast<const uint32_t*>(static_cast<std::uintptr_t>(srcArg)))));
            --count;
            currentRecord += 8;
        }
    }
    catch (...)
    {
        int entry = firstRecord;
        while (entry != currentRecord)
        {
            MissionScript_ReleaseRecord(entry);
            entry += 8;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046C680, MissionScript_CopyRecordsRange, "auto");
