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

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Allocates the 12-byte sentinel node of an STL list (std::list::_Buynode):
// the links at +0 and +4 both point back to the node itself.
// If construction throws, the node is released and the exception rethrown.
static uint32_t* __cdecl NetCogibleDdl_AllocListSentinelNode()
{
    uint32_t* sentinelNode = nullptr;

    try {
        sentinelNode = reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(Mem_AllocDebug(0xC, 0, 0)));

        if (sentinelNode != nullptr) {
            sentinelNode[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sentinelNode));
        }

        uint32_t* secondLink = sentinelNode + 1;
        if (secondLink != nullptr) {
            *secondLink = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sentinelNode));
        }
    }
    catch (...) {
        if (sentinelNode != nullptr) {
            Mem_FreeDebug(sentinelNode);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return sentinelNode;
}

HOOK(0x00741240, NetCogibleDdl_AllocListSentinelNode, "auto");
