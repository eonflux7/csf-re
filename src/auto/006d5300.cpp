#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, sub_6D4B40, 0x006D4B40, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Calls the virtual slot 0x14 (index 5) on every 60-byte vertex record, then
// frees the record array and clears the begin/end/capacity pointers.
static int __fastcall C_00861068_ResetVertexBuffer(C_00861068* self, void* /*edx*/)
{
    uint32_t recordIndex = 0;
    int byteOffset = 0;

    for (;;) {
        uint32_t begin = CSF_AT(uint32_t, self, 0x8);
        if (begin == 0)
            break;

        uint32_t end = CSF_AT(uint32_t, self, 0xC);
        uint32_t recordCount = static_cast<uint32_t>(static_cast<int>(end - begin) / 60);
        if (recordIndex >= recordCount)
            break;

        auto recordHandler = reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, self, 0)[5]);
        uint32_t* record = reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8) + static_cast<uint32_t>(byteOffset)));
        recordHandler(self, nullptr, record);

        ++recordIndex;
        byteOffset += 60;
    }

    uint32_t arrayBegin = CSF_AT(uint32_t, self, 0x8);
    if (arrayBegin != 0) {
        sub_6D4B40(static_cast<int>(arrayBegin), static_cast<int>(CSF_AT(uint32_t, self, 0xC)));
        if (CSF_AT(uint32_t, self, 0x8) != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8))));
    }

    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    self->field_0018 = 0;
    self->field_0014 = 0;
    return 0;
}

HOOK(0x006D5300, C_00861068_ResetVertexBuffer, "auto");
