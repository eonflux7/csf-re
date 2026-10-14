#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReadPlayerStateTag, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CMenuMpStats_RestorePlayerState, 0x00740B90, (CMenuMpStats* self, void* , uint32_t a2, int a3));

static int __fastcall CMenuMpStats_ReadAndRestorePlayerState(
    CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    uint32_t stateTag = reinterpret_cast<uint32_t>(self);
    C_008622e8* source = arg0;

    ReadPlayerStateTag(source, nullptr, reinterpret_cast<char*>(&stateTag));
    NetCogibleDdl_ReadIntoSlot(
        source, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&arg0)));

    return CMenuMpStats_RestorePlayerState(
        self, nullptr, stateTag,
        static_cast<int>(reinterpret_cast<uintptr_t>(arg0)));
}

HOOK(0x00745490, CMenuMpStats_ReadAndRestorePlayerState, "auto");
