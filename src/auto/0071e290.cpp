#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiBuilder::ClearHandler(int index)
// 0x0071E290: destroys the keyword-handler object stored in the builder's handler
// table at [index] (virtual slot 0 with argument 1), then nulls the entry.
static uint32_t __fastcall CGuiBuilder_ClearHandler(CGuiBuilder* self, void* /*edx*/, int index)
{
    uint32_t handlerTable = self->field_000c;
    void* handler = *reinterpret_cast<void**>(handlerTable + 4 * index);
    if (handler)
    {
        void** handlerVtbl = *reinterpret_cast<void***>(handler);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(handlerVtbl[0])(handler, nullptr, 1);
        handlerTable = self->field_000c;
        *reinterpret_cast<uint32_t*>(handlerTable + 4 * index) = 0;
    }
    return handlerTable;
}

HOOK(0x0071E290, CGuiBuilder_ClearHandler, "auto");
