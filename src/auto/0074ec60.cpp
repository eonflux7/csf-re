#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint32_t, g_NetControlClassId, 0x008BC17C);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));

static int NetControlDDL_SetClassIdAndCreate(int classId)
{
    g_NetControlClassId = static_cast<std::uint32_t>(classId);

    void* object = AllocateGatheringStorage(
        32, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 62);

    if (object)
    {
        RootDO_Construct(object, nullptr, classId);
        CSF_AT(void**, object, 0) = CSF_VTBL(0x008685F8);
    }

    // Original tail-jumps to 0x661DC0: mov eax,[ecx]; push 0; call [eax+8]; ret.
    // Inlined here: vtable slot 2 (byte offset 8 / 4) called with this=object and one 0 argument.
    using SlotFn = int(__fastcall*)(void* self, void* edx, int arg);
    SlotFn slot2 = reinterpret_cast<SlotFn>(CSF_AT(void**, object, 0)[8 / 4]);
    return slot2(object, nullptr, 0);
}

HOOK(0x0074EC60, NetControlDDL_SetClassIdAndCreate, "auto");
