#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __stdcall, InitTripleFromSource, 0x0044AEB0, (uint32_t* a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static int __cdecl InitRecordRange12(
    int firstRecord, int endRecord, int destRecord, int context, int /*unused*/)
{
    std::uint32_t source = static_cast<std::uint32_t>(firstRecord);
    std::uint32_t destination = static_cast<std::uint32_t>(destRecord);

    try {
        while (source != static_cast<std::uint32_t>(endRecord)) {
            // The disassembly also supplies context in ECX.
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*, uint32_t*, int)>(
                InitTripleFromSource)(
                    reinterpret_cast<void*>(static_cast<std::uintptr_t>(context)),
                    nullptr,
                    reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(destination)),
                    static_cast<int>(source));
            destination += 12;
            source += 12;
        }
    } catch (...) {
        std::uint32_t cursor = static_cast<std::uint32_t>(destRecord);
        while (cursor != destination) {
            void* record = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(cursor));
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, record, 0)[0])(record, nullptr, 0);
            cursor += 12;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x0044AEE0, InitRecordRange12, "auto");
