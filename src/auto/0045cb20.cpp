#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ResolveHandleToValuePtr, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsSceneEntityReady, 0x00755810, (unsigned int a1));

static int __stdcall StoreResolvedHandleResult(int a1, int a2, int a3) {
  (void)a2;

  // *(*(a1 + 4) + 4) = -1
  int* outCell = reinterpret_cast<int*>(*reinterpret_cast<int*>(a1 + 4));
  outCell[1] = -1;

  // v3 = *(*(int*)a3 + 4); v4 = *(v3 + 12)
  int* entry = reinterpret_cast<int*>(*reinterpret_cast<int*>(a3));
  int* record = reinterpret_cast<int*>(entry[1]);
  int recordIndex = record[3];

  int result = reinterpret_cast<int>(record) + 4;

  if (recordIndex >= 0) {
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
    uint8_t* sceneEntity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));

    if (sceneEntity != nullptr) {
      result = IsSceneEntityReady(static_cast<unsigned int>(CSF_AT(int, sceneEntity, 80)));

      if (static_cast<uint8_t>(result) != 0) {
        uint32_t controlWord = CSF_AT(uint32_t, sceneEntity, 68);
        void* controlObj = reinterpret_cast<void*>(controlWord);

        if (controlObj != nullptr) {
          result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
              CSF_AT(void**, controlObj, 0)[40])(controlObj, nullptr);

          if (result != 0) {
            // Reload the control object: the vtable call may have changed the entry.
            void* controlObjAgain = reinterpret_cast<void*>(CSF_AT(uint32_t, sceneEntity, 68));
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, controlObjAgain, 0)[50])(controlObjAgain, nullptr, 0);

            if (result != 0) {
              // Re-read the output cell pointer after the calls.
              int* outCellAgain = reinterpret_cast<int*>(*reinterpret_cast<int*>(a1 + 4));
              int valuePtr = ResolveHandleToValuePtr(reinterpret_cast<void*>(result), nullptr);
              result = *reinterpret_cast<int*>(valuePtr);
              outCellAgain[1] = result;
            }
          }
        }
      }
    }
  }

  return result;
}

HOOK(0x0045CB20, StoreResolvedHandleResult, "auto");
