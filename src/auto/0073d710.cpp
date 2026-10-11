#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void, __fastcall, NetSession_AdvanceIterator, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetIteratorTraversalMode, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_InitializeIteratorState, 0x0064B9B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, NetSession_ResetIteratorLink, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, NetSession_AssignIteratorSentinel, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, NetSession_CreateSelectionContext, 0x0065E250, (char a1, char a2));
CSF_FUNC(uint32_t*, __fastcall, NetSession_ConstructSelectionSentinel, 0x00662BA0, (void* self, void* edx, int a2));

static std::uint32_t* __fastcall NetSession_InitializeSelectionIterator(
    std::uint32_t* self, void* /*edx*/)
{
    alignas(8) std::uint8_t resetValueBytes[0x4];
    auto& resetValue = *reinterpret_cast<std::uint32_t*>(resetValueBytes);

    NetSession_InitializeIteratorState(self, nullptr, 0, 0);

    const int context = NetSession_CreateSelectionContext(1, 1);
    CSF_AT(std::uint32_t, self, 0) = static_cast<std::uint32_t>(context);
    if (!context)
    {
        resetValue = 0;
        NetSession_ResetIteratorLink(
            reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, &resetValue);
    }

    void* storage = AllocateGatheringStorage(
        20,
        CSF_STR(0x00867658, "C:\\ARCHIV~1\\Quazal\\NetZ\\Win32\\include\\ObjDup/SelectionIteratorTemplate.h"),
        128);

    std::uint32_t* sentinel = nullptr;
    if (storage)
        sentinel = NetSession_ConstructSelectionSentinel(storage, nullptr, 1);

    NetSession_AssignIteratorSentinel(
        self, nullptr, reinterpret_cast<int>(sentinel));

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, sentinel, 0)[8 / 4])(sentinel, nullptr);

    if (CSF_AT(std::uint32_t, self, 0))
    {
        NetSession_AdvanceIterator(self, nullptr);
        NetSession_SetIteratorTraversalMode(self, nullptr, 1);
    }
    else
    {
        resetValue = 0;
        NetSession_ResetIteratorLink(
            reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, &resetValue);
    }

    return self;
}

HOOK(0x0073D710, NetSession_InitializeSelectionIterator, "auto");
