#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __stdcall, InitScriptRecordFromSource, 0x0046EEE0, (uint32_t* a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static std::uint32_t* __cdecl InitRecordRangeFromSource(
    int firstSource, int endSource, int firstDest, int context, int /*unused*/)
{
    std::uint32_t source = static_cast<std::uint32_t>(firstSource);
    std::uint32_t destination = static_cast<std::uint32_t>(firstDest);

    try {
        while (source != static_cast<std::uint32_t>(endSource)) {
            // The disassembly supplies context in ECX; dest and source are pushed as stack args.
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*, int, int)>(
                InitScriptRecordFromSource)(
                    reinterpret_cast<void*>(static_cast<std::uintptr_t>(context)),
                    nullptr,
                    static_cast<int>(destination),
                    static_cast<int>(source));
            destination += 12;
            source += 12;
        }
    } catch (...) {
        // Landing pad: destroy the records constructed so far (vtable slot 0, flag 0).
        std::uint32_t cursor = static_cast<std::uint32_t>(firstDest);
        while (cursor != destination) {
            void* record = reinterpret_cast<void*>(static_cast<std::uintptr_t>(cursor));
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, record, 0)[0])(record, nullptr, 0);
            cursor += 12;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(destination));
}

HOOK(0x0046EF80, InitRecordRangeFromSource, "auto");
