#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00840f48_IsActive, 0x0051C0B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, NetSession_ClearListContents, 0x006508A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ClearMapContents, 0x00652810, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ClearMemberList, 0x006A6FB0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_ClearGatheringList, 0x006A7000, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0085fe28_Deactivate, 0x006AA760, (void* self, void* edx));

static void __fastcall C_0085fe28_Destroy(C_0085fe28* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0085FE28);
    if (reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_VTBL(0x0085FE34)[0])(self, nullptr))
    {
        C_0085fe28_Deactivate(self, nullptr);
        self->field_0004 = 0;
    }

    if (self->field_031c >= 0x10 && self->field_0308)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0308));
    self->field_031c = 15;
    self->field_0318 = 0;
    reinterpret_cast<uint8_t*>(&self->field_0308)[0] = 0;

    if (self->field_0300 >= 0x10 && self->field_02ec)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_02ec));
    self->field_0300 = 15;
    self->field_02fc = 0;
    reinterpret_cast<uint8_t*>(&self->field_02ec)[0] = 0;

    if (self->field_02e4 >= 0x10 && self->field_02d0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_02d0));
    self->field_02e4 = 15;
    self->field_02e0 = 0;
    reinterpret_cast<uint8_t*>(&self->field_02d0)[0] = 0;

    CSF_AT(void*, self, 0x84) = CSF_VTBL(0x0085FE20);
    void* ownedObject = reinterpret_cast<void*>(self->field_0088);
    if (ownedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
    }

    NetGatheringDdl_ClearMemberList(reinterpret_cast<uint8_t*>(self) + 0x70, nullptr);
    int node = CSF_AT(int, self, 0x74);
    uint32_t* allocator = NetSession_GetNodeAllocator();
    NetSession_FreeListNode(reinterpret_cast<int>(allocator), node, 7);
    CSF_AT(int, self, 0x74) = 0;

    NetSession_ClearMapContents(reinterpret_cast<uint8_t*>(self) + 0x60, nullptr);
    node = CSF_AT(int, self, 0x64);
    allocator = NetSession_GetNodeAllocator();
    NetSession_FreeListNode(reinterpret_cast<int>(allocator), node, 7);
    CSF_AT(int, self, 0x64) = 0;

    NetSession_ClearListContents(reinterpret_cast<uint8_t*>(self) + 0x50, nullptr);
    node = static_cast<int>(self->field_0054);
    allocator = NetSession_GetNodeAllocator();
    NetSession_FreeListNode(reinterpret_cast<int>(allocator), node, 7);
    self->field_0054 = 0;

    NetGatheringDdl_ClearGatheringList(reinterpret_cast<uint8_t*>(self) + 0x40, nullptr);
    node = CSF_AT(int, self, 0x44);
    allocator = NetSession_GetNodeAllocator();
    NetSession_FreeListNode(reinterpret_cast<int>(allocator), node, 7);
    CSF_AT(int, self, 0x44) = 0;

    self->vtbl = CSF_VTBL(0x0085FD08);
}

HOOK(0x006A71F0, C_0085fe28_Destroy, "auto");
