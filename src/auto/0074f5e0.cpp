#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(std::uint32_t, g_DdlObjectClassId, 0x008BC180);

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RootDO_InitializeDefault, 0x00661DC0, (void* self, void* edx));

static int __cdecl NetPlayerListDDL_Create(int classId)
{
    g_DdlObjectClassId = static_cast<std::uint32_t>(classId);

    void* object = AllocateGatheringStorage(
        32,
        CSF_STR(0x00868698, ".\\PCGenerados\\NetPlayerListDDL.cpp"),
        54);

    if (object)
    {
        RootDO_Construct(object, nullptr, classId);
        CSF_AT(void**, object, 0) = CSF_VTBL(0x00868708);
    }

    return RootDO_InitializeDefault(object, nullptr);
}

HOOK(0x0074F5E0, NetPlayerListDDL_Create, "auto");
