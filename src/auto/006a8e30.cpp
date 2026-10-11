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

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(char*, __cdecl, NodeAllocator_AllocBlock, 0x00647070, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void, __stdcall, NetGatheringDdl_CleanupPartialRecord, 0x006A6D80, (int a2, int _C));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Allocates a 12-byte record from the NetSession node allocator and stores
// (a2, a3, *a4) into its first three dwords. Returns the record or null.
static char* __fastcall NetGatheringDdl_CreateTriple(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    char* result = nullptr;
    try {
        uint32_t* nodeAllocator = NetSession_GetNodeAllocator();
        result = NodeAllocator_AllocBlock(reinterpret_cast<int>(nodeAllocator), 12,
                                          reinterpret_cast<int>(CSF_STR(0x0085802C, "Unknown")), 0, 7);
        if (result) {
            *reinterpret_cast<int*>(result) = a2;
            *reinterpret_cast<int*>(result + 4) = a3;
            *reinterpret_cast<uint32_t*>(result + 8) = *a4;
        }
    } catch (...) {
        NetGatheringDdl_CleanupPartialRecord(reinterpret_cast<int>(result), 1);
        throw;
    }
    return result;
}

HOOK(0x006A8E30, NetGatheringDdl_CreateTriple, "auto");
