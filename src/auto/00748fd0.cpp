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

// Exception-frame unwind data referenced by the original's EH stub (not used by the body).
CSF_GLOBAL(uint32_t, g_EhUnwindInfo, 0x00892F24);

// Allocates a 72-byte (0x48) list head node and self-links it, as std::list does for its
// sentinel: the first dword and the second dword both point back to the node itself.
// If construction throws, the node is released and the exception rethrown.
static uint32_t* __cdecl ListHeadNode_AllocSentinel72Bytes()
{
    uint32_t* headNode = nullptr;

    (void)g_EhUnwindInfo;

    try {
        headNode = reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(Mem_AllocDebug(0x48, 0, 0)));

        if (headNode != nullptr) {
            headNode[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(headNode));
        }

        uint32_t* secondLink = headNode + 1;
        if (secondLink != nullptr) {
            *secondLink = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(headNode));
        }
    }
    catch (...) {
        if (headNode != nullptr) {
            Mem_FreeDebug(headNode);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return headNode;
}

HOOK(0x00748FD0, ListHeadNode_AllocSentinel72Bytes, "auto");
