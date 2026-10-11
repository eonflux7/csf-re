#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGathering_SetTemporaryListCount, 0x00651160, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char*, __fastcall, NetGathering_CreateTemporaryListNode, 0x006524B0, (void* self, void* edx, int a2, int a3, void** a4));
CSF_FUNC(int, __fastcall, NetGathering_ClearTemporaryList, 0x00652810, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, NetGathering_CreateTemporaryListHead, 0x00656DE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGathering_CopyUrlString, 0x0066FBB0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetGathering_QueryMonitorCount, 0x007A0C70, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(int, __cdecl, _fclose, 0x007B01D7, (void* Stream));
CSF_FUNC(int, __cdecl, _fprintf, 0x007B0228, (void* Stream, void* Format, ...));
CSF_FUNC(void*, __cdecl, _fopen, 0x007B2FA5, (char* FileName, char* Mode));

CSF_GLOBAL(char, g_MonitorLogSuffix, 0x008BC0A0);

static void* __fastcall S_006aaae0_LogFailedMonitorUrls(
    S_006aaae0* self, void* /*edx*/)
{
    void* current = CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), 0);

    alignas(8) uint8_t filenameBytes[0x80];
    alignas(8) uint8_t firstStringBytes[0x8];
    alignas(8) uint8_t secondStringBytes[0x4];
    alignas(8) uint8_t temporaryListBytes[0x18];
    C_008310b8& firstString =
        *reinterpret_cast<C_008310b8*>(firstStringBytes);

    std::memset(filenameBytes, 0, sizeof(filenameBytes));
    firstString.field_0004 = reinterpret_cast<uint32_t>(self);

    GetGlobalDword_73C020();
    _sprintf(reinterpret_cast<char*>(filenameBytes),
             CSF_STR(0x0085FFF0, "LogFailedMonitorURLs%s.txt"),
             &g_MonitorLogSuffix);
    void* stream = _fopen(reinterpret_cast<char*>(filenameBytes),
                         const_cast<char*>(CSF_STR(0x0084B9CC, "a")));
    if (!stream)
        return stream;

    _fprintf(stream, const_cast<char*>(
        CSF_STR(0x0085FFC8, "------------------------------------\n")));

    if (current != reinterpret_cast<void*>(self->field_000c))
    {
        void* sentinel;
        do
        {
            CSF_AT(void*, temporaryListBytes, 0x4) =
                NetGathering_CreateTemporaryListHead(temporaryListBytes, nullptr);
            CSF_AT(int, temporaryListBytes, 0x8) = 0;

            void* head = CSF_AT(
                void*, CSF_AT(void*, temporaryListBytes, 0x4), 0);
            void* url = reinterpret_cast<uint8_t*>(current) + 8;
            char* inserted = NetGathering_CreateTemporaryListNode(
                temporaryListBytes, nullptr,
                reinterpret_cast<int>(head),
                CSF_AT(int, head, 0x4),
                reinterpret_cast<void**>(url));
            NetGathering_SetTemporaryListCount(temporaryListBytes, nullptr, 1);
            CSF_AT(char*, head, 0x4) = inserted;
            CSF_AT(char*, CSF_AT(void*, inserted, 0x4), 0) = inserted;

            uint32_t* copiedString =
                NetGathering_CopyUrlString(url, nullptr, secondStringBytes);
            const char* text = reinterpret_cast<const char*>(*copiedString);
            S_006aaae0* owner =
                reinterpret_cast<S_006aaae0*>(firstString.field_0004);
            int count = NetGathering_QueryMonitorCount(
                reinterpret_cast<void*>(owner->field_0028), nullptr,
                reinterpret_cast<int>(temporaryListBytes));
            _fprintf(stream,
                     const_cast<char*>(CSF_STR(0x0085FFB4, "%30s ....... %4d\n")),
                     text, count);
            DestroyTemporaryString(secondStringBytes, nullptr);

            NetGathering_CopyUrlString(url, nullptr, firstStringBytes);
            NetGathering_QueryMonitorCount(
                reinterpret_cast<void*>(owner->field_0028), nullptr,
                reinterpret_cast<int>(temporaryListBytes));
            DestroyTemporaryString(firstStringBytes, nullptr);

            current = CSF_AT(void*, current, 0);
            NetGathering_ClearTemporaryList(temporaryListBytes, nullptr);
            int headAllocation = CSF_AT(int, temporaryListBytes, 0x4);
            uint32_t* allocator = NetSession_GetNodeAllocator();
            NetSession_FreeListNode(
                reinterpret_cast<int>(allocator), headAllocation, 7);
            sentinel = reinterpret_cast<void*>(owner->field_000c);
            CSF_AT(void*, temporaryListBytes, 0x4) = nullptr;
        }
        while (current != sentinel);
    }

    return reinterpret_cast<void*>(_fclose(stream));
}

HOOK(0x006AAAE0, S_006aaae0_LogFailedMonitorUrls, "auto");
