#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

typedef int CoreZlibLog_HuffmanBuilder(
    uint32_t* bits, uint32_t* lengths, uint32_t count,
    uint32_t simpleCount, const uint32_t* bases,
    const uint32_t* extraBits, int* tree, void* tableStorage,
    uint32_t* tableEntries, uint32_t* workspace)
    __attribute__((cdecl, regparm(1)));

static CoreZlibLog_HuffmanBuilder* const CoreZlibLog_BuildHuffmanTable =
    reinterpret_cast<CoreZlibLog_HuffmanBuilder*>(
        CSF_ADDR_FN(0x006A6130));

static int __cdecl CoreZlibLog_BuildDynamicInflateTrees(
    uint32_t literalCount, uint32_t distanceCount, uint32_t* lengths,
    uint32_t* literalBits, uint32_t* distanceBits,
    int* literalTree, int* distanceTree, void* tableStorage, void* stream)
{
    using Allocate = void* (__cdecl*)(void*, uint32_t, uint32_t);
    using Release = void (__cdecl*)(void*, void*);

    uint32_t tableEntries = 0;
    auto* workspace = static_cast<uint32_t*>(
        CSF_AT(Allocate, stream, 0x20)(
            CSF_AT(void*, stream, 0x28), 288, 4));
    if (!workspace)
        return -4;

    int status = CoreZlibLog_BuildHuffmanTable(
        literalBits, lengths, literalCount, 0x101,
        reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x0085FA28)),
        reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x0085FAA8)),
        literalTree, tableStorage, &tableEntries, workspace);

    if (status == -3)
    {
        void* opaque = CSF_AT(void*, stream, 0x28);
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FC84, "oversubscribed literal/length tree");
        CSF_AT(Release, stream, 0x24)(opaque, workspace);
        return -3;
    }

    if (status != 0 || *literalBits == 0)
    {
        if (status != -4)
        {
            CSF_AT(const char*, stream, 0x18) =
                CSF_STR(0x0085FC64, "incomplete literal/length tree");
            status = -3;
        }
        CSF_AT(Release, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), workspace);
        return status;
    }

    status = CoreZlibLog_BuildHuffmanTable(
        distanceBits, &lengths[literalCount], distanceCount, 0,
        reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x0085FB28)),
        reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x0085FBA0)),
        distanceTree, tableStorage, &tableEntries, workspace);

    if (status == -3)
    {
        void* opaque = CSF_AT(void*, stream, 0x28);
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FCE8, "oversubscribed distance tree");
        CSF_AT(Release, stream, 0x24)(opaque, workspace);
        return -3;
    }

    if (status == -5)
    {
        void* opaque = CSF_AT(void*, stream, 0x28);
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FCCC, "incomplete distance tree");
        CSF_AT(Release, stream, 0x24)(opaque, workspace);
        return -3;
    }

    if (status == 0 && (*distanceBits != 0 || literalCount <= 0x101))
    {
        CSF_AT(Release, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), workspace);
        return 0;
    }

    if (status != -4)
    {
        CSF_AT(const char*, stream, 0x18) =
            CSF_STR(0x0085FCA8, "empty distance tree with lengths");
        status = -3;
    }

    CSF_AT(Release, stream, 0x24)(
        CSF_AT(void*, stream, 0x28), workspace);
    return status;
}

HOOK(0x006A6650, CoreZlibLog_BuildDynamicInflateTrees, "auto");
