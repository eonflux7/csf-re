#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_ReleaseAllocation, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, Sub_6687B0, 0x006687B0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_DestroyEmbeddedState, 0x0066F150, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __fastcall, NetSession_ClearSessionState, 0x0073ECC0, (void* self, void* edx));

static char __fastcall S_0073ecc0_Destroy(S_0073ecc0* self, void* /*edx*/)
{
    // The packet also lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x0073ECC0);

    NetSession_ClearSessionState(self, nullptr);
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    NetSession_DestroyEmbeddedState(bytes + 0x160, nullptr);

    if (self->field_00f8 >= 0x10 && self->field_00e4 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_00e4));

    self->field_00f8 = 15;
    self->field_00f4 = 0;
    reinterpret_cast<unsigned char*>(&self->field_00e4)[0] = 0;

    Sub_6687B0(bytes + 0xAC, nullptr);
    int allocation = CSF_AT(int, self, 0xB0);
    uint32_t* allocator = NetSession_GetAllocator();
    NetSession_ReleaseAllocation(reinterpret_cast<int>(allocator), allocation, 7);
    CSF_AT(uint32_t, self, 0xB0) = 0;

    Sub_6687B0(bytes + 0x9C, nullptr);
    allocation = CSF_AT(int, self, 0xA0);
    allocator = NetSession_GetAllocator();
    NetSession_ReleaseAllocation(reinterpret_cast<int>(allocator), allocation, 7);
    CSF_AT(uint32_t, self, 0xA0) = 0;

    const uint16_t emptyCharacter = 0;
    uint32_t result = self->field_0094;
    if (result >= 8)
    {
        result = self->field_0080;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    self->field_0094 = 7;
    self->field_0090 = 0;
    std::memcpy(&self->field_0080, &emptyCharacter, sizeof(emptyCharacter));

    if (self->field_0078 >= 8)
    {
        result = self->field_0064;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    self->field_0078 = 7;
    self->field_0074 = 0;
    std::memcpy(&self->field_0064, &emptyCharacter, sizeof(emptyCharacter));

    if (self->field_005c >= 8)
    {
        result = self->field_0048;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    self->field_005c = 7;
    self->field_0058 = 0;
    std::memcpy(&self->field_0048, &emptyCharacter, sizeof(emptyCharacter));

    if (self->field_0040 >= 8)
    {
        result = self->field_002c;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    self->field_0040 = 7;
    self->field_003c = 0;
    std::memcpy(&self->field_002c, &emptyCharacter, sizeof(emptyCharacter));

    if (self->field_0024 >= 8)
    {
        result = self->field_0010;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    self->field_0020 = 0;
    self->field_0024 = 7;
    std::memcpy(&self->field_0010, &emptyCharacter, sizeof(emptyCharacter));

    return static_cast<char>(result);
}

HOOK(0x0073EEA0, S_0073ecc0_Destroy, "auto");
