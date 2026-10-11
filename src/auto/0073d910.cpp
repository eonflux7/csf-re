#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void, __fastcall, NetSession_ResetSelection, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetSelectionMode, 0x0064B900, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_InitializeSelectionBase, 0x0064B9B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, NetSession_AssignSelectionCursor, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, NetSession_AttachSelectionIterator, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, NetSession_CreateSelectionHandle, 0x0065E250, (char a1, char a2));
CSF_FUNC(uint32_t*, __fastcall, NetSession_ConstructPlayerSelectionIterator, 0x00662BA0, (void* self, void* edx, int a2));

CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);

static std::uint32_t* __fastcall NetSession_InitializeGameSelection(
    std::uint32_t* self, void* /*edx*/)
{
    NetSession_InitializeSelectionBase(self, nullptr, 0, 0);

    const int handle = NetSession_CreateSelectionHandle(1, 1);
    CSF_AT(std::uint32_t, self, 0) = static_cast<std::uint32_t>(handle);

    std::uint32_t emptyCursor;
    if (!handle)
    {
        emptyCursor = 0;
        NetSession_AssignSelectionCursor(
            reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, &emptyCursor);
    }

    void* storage = AllocateGatheringStorage(
        20,
        CSF_STR(0x00867658, "C:\\ARCHIV~1\\Quazal\\NetZ\\Win32\\include\\ObjDup/SelectionIteratorTemplate.h"),
        128);

    std::uint32_t* iterator = nullptr;
    if (storage)
    {
        iterator = NetSession_ConstructPlayerSelectionIterator(
            storage, nullptr, static_cast<int>(g_NetGameClassId));
    }

    NetSession_AttachSelectionIterator(
        self, nullptr, reinterpret_cast<int>(iterator));

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, iterator, 0)[2])(
        iterator, nullptr);

    if (CSF_AT(std::uint32_t, self, 0))
    {
        NetSession_ResetSelection(self, nullptr);
        NetSession_SetSelectionMode(self, nullptr, 1);
    }
    else
    {
        emptyCursor = 0;
        NetSession_AssignSelectionCursor(
            reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, &emptyCursor);
    }

    return self;
}

HOOK(0x0073D910, NetSession_InitializeGameSelection, "auto");
