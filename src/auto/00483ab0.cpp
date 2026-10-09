#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Ejecuta la accion de respaldo (slot 0x11C con argumento 8) cuando la accion
// actual no esta lista o el estado asociado no es el esperado (valor 11).
static int __fastcall CCharActionFsm_ReturnFallbackActionIfIdle(CCharActionFsm* self, void* /*edx*/)
{
    // Objeto de accion devuelto por el slot 0xD8 (primera llamada virtual).
    void* actionObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));

    // Llamada virtual slot 0x2C (44 / 4 = 11) sobre el objeto de accion; devuelve bool en al.
    int result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, actionObj, 0)[11])(actionObj, nullptr);

    if (static_cast<uint8_t>(result) != 0)
    {
        uint32_t stateObjField = self->field_009c;
        if (stateObjField == 0)
            return self->vSlot11C(8);

        // Llamada virtual slot 0x4C (76 / 4 = 19) sobre el objeto de estado.
        void* stateObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_009c));
        result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[19])(stateObj, nullptr);
        if (result != 11)
            return self->vSlot11C(8);
    }
    return result;
}

HOOK(0x483AB0, CCharActionFsm_ReturnFallbackActionIfIdle, "auto");
