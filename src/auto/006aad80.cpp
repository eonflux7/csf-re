#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseSessionListEntries, 0x00652810, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, InitializeSessionList, 0x00656DE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SetSessionReference, 0x006A8DD0, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, PopulateSessionList, 0x006AA900, (void* self, void* edx, void** a2, void* a3));
CSF_FUNC(void*, __fastcall, ResetGatheringSession, 0x006AA970, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, C_0085839c_TryAdvanceSession, 0x0073CB00, (C_0085839c* self, void* , int a2));
CSF_FUNC(char, __fastcall, StartResourceRequest, 0x00798FF0, (void* self, void* edx, void* a2, int a3, int a4, int a5));
CSF_FUNC(void*, __fastcall, ConstructSessionOperation, 0x0079FEF0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, StartSessionOperation, 0x0079FFF0, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, FindGatheringSession, 0x007A0C70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, DestroyTransportStorage, 0x007A14C0, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall NetGatheringDdl_UpdateSessionTransition(
    void* self, void* /*edx*/, float elapsedTime)
{
    auto* objectBytes = static_cast<uint8_t*>(self);
    alignas(8) uint8_t sessionBytes[0x14];

    float remainingTime = CSF_AT(float, self, 0x324) - elapsedTime;
    CSF_AT(uint8_t, sessionBytes, 0x13) = 0;
    CSF_AT(float, self, 0x324) = remainingTime;
    if (!(remainingTime < g_fZero))
        return;

    if (FindGatheringSession(
            CSF_AT(void*, self, 0x320), nullptr,
            reinterpret_cast<int>(objectBytes + 0x60)) == -1)
    {
        CSF_AT(int, self, 0x2B0) = 1229731640;
        ResetGatheringSession(self, nullptr);
        CSF_AT(uint8_t, sessionBytes, 0x13) = 1;
    }
    else
    {
        char* listNode = InitializeSessionList(sessionBytes, nullptr);
        void* sessionReference = CSF_AT(void*, self, 0x32C);
        CSF_AT(char*, sessionBytes, 0x4) = listNode;
        CSF_AT(int, sessionBytes, 0x8) = 0;

        PopulateSessionList(
            self, nullptr,
            CSF_AT(void**, CSF_AT(void*, sessionReference, 0x4), 0xC),
            sessionBytes);

        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB8 / 4])(
            self, nullptr);

        auto* sessionManager =
            reinterpret_cast<C_0085839c*>(GetGlobalDword_73C020());
        if (C_0085839c_TryAdvanceSession(
                sessionManager, nullptr, reinterpret_cast<int>(sessionBytes)))
        {
            void* oldOperation = CSF_AT(void*, self, 0x330);
            if (oldOperation)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, oldOperation, 0)[0])(
                        oldOperation, nullptr, 1);
                CSF_AT(void*, self, 0x330) = nullptr;
            }

            void* allocation = NetZ_OperatorNew(32);
            void* operation = allocation
                ? ConstructSessionOperation(
                      allocation, nullptr, CSF_AT(int, self, 0x2C), 600)
                : nullptr;

            int operationArgument = CSF_AT(int, self, 0x8C);
            CSF_AT(void*, self, 0x330) = operation;
            StartSessionOperation(operation, nullptr, operationArgument);

            int nextSessionReference = CSF_AT(int, self, 0x32C);
            CSF_AT(uint8_t, self, 0x334) = 0;
            CSF_AT(int, self, 0x8) = 7;
            CSF_AT(int, self, 0xC) = 0;
            SetSessionReference(
                objectBytes + 0x84, nullptr, nextSessionReference);
        }
        else
        {
            CSF_AT(int, self, 0x2B0) = 1229730354;
            CSF_AT(uint8_t, sessionBytes, 0x13) = 1;
        }

        ReleaseSessionListEntries(sessionBytes, nullptr);
        int nodeToFree = CSF_AT(int, sessionBytes, 0x4);
        uint32_t* allocator = NetSession_GetNodeAllocator();
        NetSession_FreeListNode(
            reinterpret_cast<int>(allocator), nodeToFree, 7);
    }

    void* transportStorage = CSF_AT(void*, self, 0x320);
    if (transportStorage)
    {
        DestroyTransportStorage(transportStorage, nullptr);
        NetSession_ReleaseTransportStorage(
            reinterpret_cast<int>(transportStorage));
        CSF_AT(void*, self, 0x320) = nullptr;
    }

    if (CSF_AT(uint8_t, sessionBytes, 0x13))
    {
        void* allocation = NetZ_OperatorNew(112);
        void* resource =
            allocation ? ConstructResource(allocation, nullptr) : nullptr;
        CSF_AT(void*, self, 0x90) = resource;

        alignas(8) uint8_t temporaryStringBytes[0x10];
        InitializeTemporaryString(
            temporaryStringBytes, nullptr,
            reinterpret_cast<std::int32_t>(&g_byte_82E65C));

        int requestArgument = CSF_AT(int, self, 0x8C);
        void* requestResource = CSF_AT(void*, self, 0x90);
        StartResourceRequest(
            CSF_AT(void*, self, 0x2C), nullptr, requestResource,
            requestArgument, reinterpret_cast<int>(objectBytes + 0x2B4),
            reinterpret_cast<int>(temporaryStringBytes));

        DestroyTemporaryString(temporaryStringBytes, nullptr);
        CSF_AT(int, self, 0x18) = 5;
    }
}

HOOK(0x006AAD80, NetGatheringDdl_UpdateSessionTransition, "auto");
