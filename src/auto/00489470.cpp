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

// Callee bindings (by original address).
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, CanSkipRecordCreate, 0x004828D0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, AllocActionRecord, 0x00475A40, (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, AttachRecordToOwner, 0x00489390, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, MarkAsVirtualProcessorRoot, 0x00476170, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_FinishUpdate, 0x00486FC0, (void* self, void* edx));

// Binds the action record for id a2 to processor root a3 (-1 = none).
// If a record already exists for the id, forwards a3 (or 0) to its virtual
// slots 23 (+0x5C) / 24 (+0x60). Otherwise creates a new record unless
// CanSkipRecordCreate says otherwise, and attaches it to this FSM.
static void __fastcall CCharActionFsm_BindActionRecord(CCharActionFsm* self, void* /*edx*/, int a2, int a3)
{
    int recordIndex = FindRecordIndexById(self, nullptr, a2);

    if (recordIndex == -1) {
        if (!CanSkipRecordCreate(self, nullptr, a2, a3)) {
            C_00862490* record = reinterpret_cast<C_00862490*>(
                AllocActionRecord(1, a2, (int)self->field_0008));
            AttachRecordToOwner(self, nullptr, record);
            CSF_AT(uint8_t, record, 0x7C) = 1;
            if (a3 != -1) {
                MarkAsVirtualProcessorRoot(record, nullptr, reinterpret_cast<void*>(a3));
            }
        }
    } else {
        uint32_t* recordTable = reinterpret_cast<uint32_t*>(self->field_0078);
        void* existing = reinterpret_cast<void*>(recordTable[recordIndex]);
        if (a3 == -1) {
            // vtable slot 0x60 / 4 = 24, pushed argument 0
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, existing, 0)[24])(existing, nullptr, 0);
        } else {
            // vtable slot 0x5C / 4 = 23, pushed argument a3
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, existing, 0)[23])(existing, nullptr, a3);
        }
    }

    CCharActionFsm_FinishUpdate(self, nullptr);
}

HOOK(0x489470, CCharActionFsm_BindActionRecord, "auto");
