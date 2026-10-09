#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __stdcall, InitRecordFromSource, 0x00453BD0, (int a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static int __cdecl ApplyToRecordRange(
    int firstRecord, int endRecord, int destRecord, int context, int /*unused*/)
{
    std::uint32_t source = static_cast<std::uint32_t>(firstRecord);
    std::uint32_t destination = static_cast<std::uint32_t>(destRecord);

    try {
        while (source != static_cast<std::uint32_t>(endRecord)) {
            // The disassembly also supplies context in ECX.
            reinterpret_cast<void*(__fastcall*)(void*, void*, int, int)>(
                InitRecordFromSource)(
                    reinterpret_cast<void*>(static_cast<std::uintptr_t>(context)),
                    nullptr,
                    static_cast<int>(destination),
                    static_cast<int>(source));
            destination += 48;
            source += 48;
        }
    } catch (...) {
        std::uint32_t cursor = static_cast<std::uint32_t>(destRecord);
        while (cursor != destination) {
            void* record = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(cursor));
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, record, 0)[0])(record, nullptr, 0);
            cursor += 48;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00453C20, ApplyToRecordRange, "auto");
