#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __cdecl, sub_6BBFF0, 0x006BBFF0, ());
CSF_FUNC(int, __fastcall, sub_6BC010, 0x006BC010, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, sub_6B6AD0, 0x006B6AD0, ());
CSF_FUNC(uint32_t*, __fastcall, sub_6B6B00, 0x006B6B00, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, sub_6F7030, 0x006F7030, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_6CA970, 0x006CA970, (void* self, void* edx));

static char __fastcall C_00860bb8_InitStreamingState(C_00860bb8* self, void* /*edx*/)
{
    uint32_t sceneHandle = reinterpret_cast<uintptr_t>(sub_6BBFF0());
    self->field_0008 = sceneHandle;
    sub_6BC010(reinterpret_cast<void*>(static_cast<uintptr_t>(sceneHandle)), nullptr);

    void* streamSource = sub_6B6AD0();
    self->field_000c = reinterpret_cast<uintptr_t>(streamSource);
    sub_6B6B00(streamSource, nullptr);

    void* block = Mem_Alloc(28);
    uint32_t* helper = nullptr;
    if (block)
        helper = sub_6F7030(block, nullptr, reinterpret_cast<int>(self));
    self->field_0070 = reinterpret_cast<uintptr_t>(helper);

    uint32_t loadedFlag = self->field_0010;
    self->field_0098 = 0;
    self->field_009c = 0;
    CSF_AT(uint8_t, self, 0x94) = 1;
    if (!loadedFlag)
        sub_6CA970(self, nullptr);
    return 1;
}

HOOK(0x006CB330, C_00860bb8_InitStreamingState, "auto");
