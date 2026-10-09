#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetIndexedRecordAddress, 0x00464000, (void* self, void* , std::uint32_t index));
CSF_FUNC(uint8_t*, __fastcall, SinkReserveCapacity, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, StoreRecordItemFromSink, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SetRecordArrayCount, 0x00473D60, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, GetEntryAtIndex, 0x004E5B10, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(uint32_t, __fastcall, GetSubItemCount, 0x004E6410, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall(this, a2, a3, a4) with 3 stack args -> ret 12.
static char* __fastcall S_00464000_CollectHandleEntries(S_00464000* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // S_00464000 vtable: this-type of the GetIndexedRecordAddress call. Not written in this function.
    (void)CSF_VTBL(0x00464000);

    // result = *(*a4 + 4) + 4  (address of the handle slot)
    uint32_t handleAddr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a4[0]), 0x04) + 4;
    char* result = reinterpret_cast<char*>(handleAddr);

    // Proceed only when the signed field at slot+8 is non-negative
    if (CSF_AT(int32_t, reinterpret_cast<uint8_t*>(handleAddr), 0x08) >= 0)
    {
        result = reinterpret_cast<char*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr)));

        if (result != nullptr)
        {
            uint32_t sceneObjectAddr = CSF_AT(uint32_t, result, 0x3C);

            if (sceneObjectAddr != 0)
            {
                // record = GetIndexedRecordAddress(this, *(a4[1] + 8)); recordData = record->+4
                int recordAddr = GetIndexedRecordAddress(
                    self, nullptr, CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a4[1]), 0x08));
                void* recordData = CSF_AT(void*, reinterpret_cast<uint8_t*>(recordAddr), 0x04);

                uint32_t entryCount = GetSubItemCount(reinterpret_cast<void*>(sceneObjectAddr), nullptr);
                result = reinterpret_cast<char*>(SetRecordArrayCount(recordData, nullptr, entryCount));

                if (entryCount != 0)
                {
                    // Local temporary object (C_008313f8 layout): vtable, buffer pointer, capacity marker.
                    uint32_t sinkObj[4];
                    CSF_AT(uint32_t, sinkObj, 0x00) = (uint32_t)CSF_VTBL(0x008313F8);
                    CSF_AT(uint32_t, sinkObj, 0x04) = 0;
                    CSF_AT(uint32_t, sinkObj, 0x08) = 0xFFFFFFFFu;
                    SinkReserveCapacity(sinkObj, nullptr, 4);

                    uint32_t vtableSlot;
                    uint32_t entryIndex = 0;
                    do
                    {
                        // Buffer pointer is re-read every iteration, before the entry lookup.
                        uint32_t sinkBufferAddr = CSF_AT(uint32_t, sinkObj, 0x04);

                        uint32_t* entryPair = GetEntryAtIndex(
                            reinterpret_cast<void*>(sceneObjectAddr), nullptr,
                            &vtableSlot, static_cast<int>(entryIndex));

                        uint8_t* sinkBuffer = reinterpret_cast<uint8_t*>(sinkBufferAddr);
                        CSF_AT(uint32_t, sinkBuffer, 0x08) = CSF_AT(uint32_t, entryPair, 0x04);
                        CSF_AT(uint32_t, sinkBuffer, 0x0C) = CSF_AT(uint32_t, entryPair, 0x08);

                        vtableSlot = (uint32_t)CSF_VTBL(0x0082D410);

                        StoreRecordItemFromSink(
                            recordData, nullptr, entryIndex,
                            static_cast<int>(reinterpret_cast<uintptr_t>(sinkObj)));
                        entryIndex++;
                    } while (entryIndex < entryCount);

                    return Object_PrepareForDestruction(reinterpret_cast<char**>(sinkObj), nullptr);
                }
            }
        }
    }

    return result;
}

HOOK(0x00464D20, S_00464000_CollectHandleEntries, "auto");
