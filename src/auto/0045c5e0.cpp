#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, IsRefHandleValid, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetRecordByIndex, 0x00485940, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetRecordScalarValue, 0x00476B20, (void* self, void* edx));

static int __stdcall WriteEntityActionScalarToOutput(void* outScalar, void* /*unusedArg*/, uint32_t* actorRef)
{
    // *(*(a1 + 4) + 4) = 0;
    CSF_AT(uint32_t, CSF_AT(void*, outScalar, 4), 4) = 0;

    // v3 = *(*a3 + 4); v4 = *(v3 + 12); result = v3 + 4;
    void* actorNode = reinterpret_cast<void*>(static_cast<uintptr_t>(*actorRef));
    void* handleOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, actorNode, 4)));
    int handleIndex = static_cast<int>(CSF_AT(uint32_t, handleOwner, 12));
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(handleOwner) + 4);

    if (handleIndex >= 0)
    {
        // result = CHandleTable::Resolve(g_pCurrentScene, v3 + 4);
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        void* entity = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        if (entity != nullptr)
        {
            // result = sub_755810(entity->field_50);
            result = IsRefHandleValid(CSF_AT(uint32_t, entity, 80));
            if (static_cast<uint8_t>(result) != 0)
            {
                // subsystem = GetValue8B0554();
                // result = subsystem->controller->vtbl[3](controller, *(*(a3[1] + 4) + 4));
                void* subsystem = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
                void* actionController = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, subsystem, 8128)));
                void* argNode = reinterpret_cast<void*>(static_cast<uintptr_t>(actorRef[1]));
                void* argLeaf = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argNode, 4)));
                uint32_t argValue = CSF_AT(uint32_t, argLeaf, 4);
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, actionController, 0)[3])(
                    actionController, nullptr, static_cast<int>(argValue));

                int resolvedId = result;
                if (resolvedId != -1)
                {
                    // Take the indexed-record path if entity->field_50 != 0, or if the
                    // state probe through controller->vtbl[2] reports the sub-state as disabled.
                    bool takeIndexedBranch = CSF_AT(uint32_t, entity, 80) != 0;
                    if (!takeIndexedBranch)
                    {
                        void* probeSubsystem = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
                        void* probeController = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, probeSubsystem, 8128)));
                        void* stateNode = reinterpret_cast<void*>(static_cast<uintptr_t>(
                            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, probeController, 0)[2])(
                                probeController, nullptr, resolvedId)));
                        takeIndexedBranch = static_cast<uint8_t>(IsSubStateEnabled(
                            static_cast<int>(CSF_AT(uint32_t, stateNode, 116)))) == 0;
                    }

                    if (takeIndexedBranch)
                    {
                        void* recordTable = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entity, 68)));
                        // result = recordTable->vtbl[48](recordTable, resolvedId);
                        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, recordTable, 0)[48])(
                            recordTable, nullptr, resolvedId);
                        if (result != -1)
                        {
                            int recordSlot = result;
                            // record = recordTable->vtbl[50](recordTable, recordSlot);
                            void* record = reinterpret_cast<void*>(static_cast<uintptr_t>(
                                ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, recordTable, 0)[50])(
                                    recordTable, nullptr, recordSlot)));
                            void* scalarHolder = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, outScalar, 4)));
                            result = GetRecordScalarValue(record, nullptr);
                            CSF_AT(float, scalarHolder, 4) = static_cast<float>(result);
                        }
                    }
                    else
                    {
                        void* stateTable = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entity, 68)));
                        result = FindRecordIndexById(stateTable, nullptr, resolvedId);
                        if (result != -1)
                        {
                            void* record = reinterpret_cast<void*>(static_cast<uintptr_t>(
                                GetRecordByIndex(stateTable, nullptr, result)));
                            void* scalarHolder = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, outScalar, 4)));
                            result = GetRecordScalarValue(record, nullptr);
                            CSF_AT(float, scalarHolder, 4) = static_cast<float>(result);
                        }
                    }
                }
            }
        }
    }

    return result;
}

HOOK(0x0045C5E0, WriteEntityActionScalarToOutput, "auto");
