#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindSlotIndex, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSlotRecord, 0x0075CF00, (void* self, void* edx, int a2));

static void* AsPtr(int value)
{
  return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int __stdcall ResolveHandleAndDispatchUpdate(int a1, int a2, uint32_t* eventArgs)
{
  (void)a1;
  (void)a2;

  uint32_t ownerRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(eventArgs[0]), 4);
  int recordCode = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(ownerRecord)), 12);
  int result = static_cast<int>(ownerRecord + 4);

  if (recordCode >= 0)
  {
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
    int handleObject = result;
    if (handleObject != 0)
    {
      uint32_t sourceRecord = eventArgs[1];
      uint32_t sourceInner = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(sourceRecord)), 4);
      int slotIndex = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(sourceInner)), 4);

      int resultArgs[2];
      resultArgs[0] = 36;
      resultArgs[1] = -1;

      if (slotIndex != -1)
      {
        void* slotTable = CSF_AT(void*, AsPtr(GetValue8B0554()), 0x1FD8);
        int foundSlot = FindSlotIndex(slotTable, nullptr, slotIndex);
        if (foundSlot != -1)
        {
          void* slotRecord = AsPtr(GetSlotRecord(slotTable, nullptr, foundSlot));
          if (CSF_AT(int, slotRecord, 0x20) == 3)
            resultArgs[1] = foundSlot;
        }
      }

      void* dispatcher = CSF_AT(void*, AsPtr(handleObject), 0x34);
      auto dispatchSlot = reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(
          CSF_AT(void**, dispatcher, 0)[42]);
      return dispatchSlot(dispatcher, nullptr, resultArgs);
    }
  }
  return result;
}

HOOK(0x00460CA0, ResolveHandleAndDispatchUpdate, "auto");
