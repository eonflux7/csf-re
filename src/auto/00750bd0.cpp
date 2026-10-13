#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(int*, __fastcall, NetChatDDL_InitBase, 0x00750CE0, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, ListHeadNode_AllocSentinel72Bytes, 0x00748FD0, ());

static char* __cdecl NetChatDDL_AllocateAndInit()
{
    // Allocate 0x468 bytes (1128) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0x468, CSF_STR(0x0086891C, ".\\PCGenerados\\NetChatDDL.cpp"), 11));
    if (!self)
        return nullptr;

    NetChatDDL_InitBase(self, nullptr);

    // Embedded sub-object at +0x440 (1088).
    uint8_t* subObject = self + 0x440;
    C_00867890_InitDefaults(subObject, nullptr);
    *reinterpret_cast<void***>(subObject) = CSF_VTBL(0x00868090);

    // Object's own vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008680A0);

    // List head at +0x45C (1116): sentinel pointer at +0x460, count at +0x464.
    uint8_t* listHead = self + 0x45C;
    CSF_AT(uint32_t*, listHead, 4) = ListHeadNode_AllocSentinel72Bytes();
    CSF_AT(uint32_t, listHead, 8) = 0;

    CSF_AT(int32_t, self, 0x44C) = -1;

    return reinterpret_cast<char*>(self);
}

HOOK(0x00750BD0, NetChatDDL_AllocateAndInit, "auto");
