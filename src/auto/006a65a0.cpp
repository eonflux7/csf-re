#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

typedef int (__cdecl *HuffmanBuilder)(
    unsigned int* rootBits, std::uint32_t* lengths,
    unsigned int count, unsigned int simpleCount,
    int baseValues, int extraBits, int* table,
    int tableStorage, unsigned int* entriesUsed,
    unsigned int* workspace) __attribute__((regparm(1)));

static HuffmanBuilder const CoreZlibLog_BuildHuffmanTable =
    reinterpret_cast<HuffmanBuilder>(CSF_ADDR_FN(0x006A6130));

static int __cdecl CoreZlibLog_BuildDynamicBitLengthsTree(
    std::uint32_t* lengths, unsigned int* rootBits,
    int* table, int tableStorage, void* stream)
{
    using Allocate = unsigned int* (__cdecl *)(
        std::uint32_t, unsigned int, unsigned int);
    using Release = void (__cdecl *)(std::uint32_t, unsigned int*);

    const std::uint32_t allocationContext =
        CSF_AT(std::uint32_t, stream, 0x28);
    unsigned int entriesUsed = 0;
    unsigned int* workspace =
        CSF_AT(Allocate, stream, 0x20)(allocationContext, 19, 4);
    if (!workspace)
        return -4;

    int result = CoreZlibLog_BuildHuffmanTable(
        rootBits, lengths, 0x13u, 0x13u, 0, 0,
        table, tableStorage, &entriesUsed, workspace);

    if (result == -3)
    {
        const std::uint32_t releaseContext =
            CSF_AT(std::uint32_t, stream, 0x28);
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FC3C, "oversubscribed dynamic bit lengths tree");
        CSF_AT(Release, stream, 0x24)(releaseContext, workspace);
        return -3;
    }

    if (result == -5 || !*rootBits)
    {
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FC18, "incomplete dynamic bit lengths tree");
        result = -3;
    }

    const std::uint32_t releaseContext =
        CSF_AT(std::uint32_t, stream, 0x28);
    CSF_AT(Release, stream, 0x24)(releaseContext, workspace);
    return result;
}

HOOK(0x006A65A0, CoreZlibLog_BuildDynamicBitLengthsTree, "auto");
