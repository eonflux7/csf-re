#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, QueryEntryListForSelection, 0x00462B10, (void* self, void* edx, uint32_t* a1, int a2, char a3, char a4));
CSF_FUNC(uint8_t*, __fastcall, ReserveScratchEntries, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, WriteOutputRecordAt, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ReserveOutputEntries, 0x00473D60, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, LookupSlotIndexForHandle, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GetSlotRecordPointer, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetEntryListFromOwner, 0x0075C260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryHandle, 0x0075D000, (void* self, void* edx, uint32_t* a2));

static char __fastcall GatherEntryRecordsIntoOutput(S_00462c00* self, void* /*edx*/, int queryKey, char queryFlagA, char queryFlagB, S_0069b1c0* outputList)
{
    // Owner object is stored at +8152 (0x1FD8) of the value returned by GetValue8B0554().
    uint8_t* gameStateBase = reinterpret_cast<uint8_t*>(GetValue8B0554());
    S_004abb90* selectionOwner = CSF_AT(S_004abb90*, gameStateBase, 8152);
    uint8_t* selectionSubObject = reinterpret_cast<uint8_t*>(selectionOwner) + 0x34;

    uint32_t* entryList = reinterpret_cast<uint32_t*>(GetEntryListFromOwner(selectionOwner, nullptr));
    QueryEntryListForSelection(self, nullptr, entryList, queryKey, queryFlagA, queryFlagB);

    if (entryList[2] != 0)
    {
        C_008313f8 scratchList;
        // Base vtable store at offset 0; C_008313f8 declares no __vftable member.
        CSF_AT(void*, &scratchList, 0) = reinterpret_cast<void*>(CSF_VTBL(0x008313F8));
        scratchList.field_0004 = 0;
        scratchList.field_0008 = 0xFFFFFFFFu;
        ReserveScratchEntries(&scratchList, nullptr, 4);

        unsigned int entryCount = entryList[2];
        ReserveOutputEntries(outputList, nullptr, entryCount);

        for (unsigned int i = 0; i < entryCount; ++i)
        {
            int resolvedHandle = ResolveEntryHandle(selectionOwner, nullptr, reinterpret_cast<uint32_t*>(entryList[0] + 8 * i));
            int slotIndex = LookupSlotIndexForHandle(selectionSubObject, nullptr, reinterpret_cast<float*>(resolvedHandle));
            if (slotIndex != -1)
            {
                int slotRecord = GetSlotRecordPointer(selectionSubObject, nullptr, slotIndex);
                uint8_t* sourceRecord = reinterpret_cast<uint8_t*>(slotRecord);
                uint8_t* scratchBytes = reinterpret_cast<uint8_t*>(scratchList.field_0004);
                CSF_AT(uint32_t, scratchBytes, 8) = CSF_AT(uint32_t, sourceRecord, 8);
                CSF_AT(uint32_t, scratchBytes, 12) = CSF_AT(uint32_t, sourceRecord, 12);
                WriteOutputRecordAt(outputList, nullptr, i, reinterpret_cast<int>(&scratchList));
            }
        }

        Object_PrepareForDestruction(reinterpret_cast<char**>(&scratchList), nullptr);
        CSF_AT(uint8_t, selectionOwner, 0x30) = 0;
        return 0;
    }

    CSF_AT(uint8_t, selectionOwner, 0x30) = 0;
    return 0;
}

HOOK(0x00462C00, GatherEntryRecordsIntoOutput, "auto");
