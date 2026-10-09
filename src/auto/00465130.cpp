#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Views a raw game address as a byte pointer.
static uint8_t* AddressToBytes(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_4F0210, 0x004F0210, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(uint8_t*, __fastcall, sub_4725C0, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_473490, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_473D60, 0x00473D60, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall ReplaceTableEntriesFromRecordRange(uint8_t* self, void* /*edx*/,
                                                        int /*unusedStackArg0*/, int /*unusedStackArg1*/,
                                                        int* descriptor)
{
    uint8_t* engineRoot = AddressToBytes(static_cast<uint32_t>(GetValue8B0554()));
    void* manager = CSF_AT(void*, engineRoot, 0x1FDC);

    // 16-byte range struct on the stack: [1]=begin, [2]=end, [3]=unused (pointer-sized slots).
    uint32_t entryRange[4];
    entryRange[1] = 0;
    entryRange[2] = 0;
    entryRange[3] = 0;

    uint32_t descriptorTarget = static_cast<uint32_t>(*descriptor);
    uint32_t innerNode = CSF_AT(uint32_t, AddressToBytes(descriptorTarget), 4);
    uint32_t propertyValue = CSF_AT(uint32_t, AddressToBytes(innerNode), 4);
    sub_4F0210(manager, nullptr, reinterpret_cast<void*>(static_cast<uintptr_t>(propertyValue)), entryRange);

    uint32_t packedIndex = CSF_AT(uint32_t, AddressToBytes(static_cast<uint32_t>(descriptor[1])), 8);
    uint32_t ownerAddress = CSF_AT(uint32_t, self, 0x20);
    if ((packedIndex & 0x800000u) != 0)
    {
        uint8_t* scene = static_cast<uint8_t*>(g_pCurrentScene);
        uint32_t sceneChild = CSF_AT(uint32_t, scene, 0xBC);
        ownerAddress = CSF_AT(uint32_t, AddressToBytes(sceneChild), 0x38);
        packedIndex &= ~0x800000u;
    }

    uint32_t tableAddress = CSF_AT(uint32_t, AddressToBytes(ownerAddress), 8);
    void* targetObject = CSF_AT(void*, AddressToBytes(tableAddress), 12u * packedIndex + 4u);

    uint32_t recordCount = 0;
    if (entryRange[1] != 0)
    {
        recordCount = static_cast<uint32_t>(static_cast<int>(entryRange[2] - entryRange[1]) / 12);
    }
    int result = static_cast<int>(sub_473D60(targetObject, nullptr, recordCount));

    if (recordCount != 0)
    {
        // Temporary 12-byte record-buffer object, laid out as:
        //   [0] vtable (0x8313F8), [4] buffer begin, [8] sentinel (-1).
        uint8_t recordBufferStorage[12];
        uint8_t* recordBuffer = recordBufferStorage;
        CSF_AT(void*, recordBuffer, 0) = reinterpret_cast<void*>(CSF_VTBL(0x008313F8));
        CSF_AT(uint32_t, recordBuffer, 4) = 0u;
        CSF_AT(uint32_t, recordBuffer, 8) = 0xFFFFFFFFu;

        sub_4725C0(recordBuffer, nullptr, 4);

        uint32_t recordOffset = 0;
        for (uint32_t i = 0; i < recordCount; ++i)
        {
            uint32_t sourceRecord = entryRange[1] + recordOffset;
            uint32_t bufferBegin = CSF_AT(uint32_t, recordBuffer, 4);
            CSF_AT(uint32_t, AddressToBytes(bufferBegin), 8) =
                CSF_AT(uint32_t, AddressToBytes(sourceRecord), 4);
            CSF_AT(uint32_t, AddressToBytes(bufferBegin), 12) =
                CSF_AT(uint32_t, AddressToBytes(sourceRecord), 8);
            sub_473490(targetObject, nullptr, i, static_cast<int>(reinterpret_cast<uintptr_t>(recordBuffer)));
            recordOffset += 12;
        }

        result = static_cast<int>(reinterpret_cast<uintptr_t>(
            Object_PrepareForDestruction(reinterpret_cast<char**>(recordBuffer), nullptr)));
    }

    uint32_t recordCursor = entryRange[1];
    if (recordCursor != 0)
    {
        uint32_t recordEnd = entryRange[2];
        if (recordCursor != recordEnd)
        {
            do
            {
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, AddressToBytes(recordCursor), 0)[0])(
                    AddressToBytes(recordCursor), nullptr, 0);
                recordCursor += 12;
            } while (recordCursor != recordEnd);
            recordCursor = entryRange[1];
        }
        if (recordCursor != 0)
        {
            return Mem_FreeDebug(AddressToBytes(recordCursor));
        }
    }
    return result;
}

HOOK(0x00465130, ReplaceTableEntriesFromRecordRange, "auto");
