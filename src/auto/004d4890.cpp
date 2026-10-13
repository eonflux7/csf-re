#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0083bc28_RefreshBeforeDispatch, 0x004D3B30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083bc28_HandleStateThree, 0x004D4050, (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, C_0083bc28_HandleStateFour, 0x004D42D0, (void* self, void* edx, int arg0));
CSF_FUNC(void, __fastcall, C_0083bc28_HandleStateFive, 0x004D45F0, (void* self, void* edx, float a2));

static void __fastcall C_0083bc28_DispatchStateHandler(C_0083bc28* self, void* /*edx*/, int a2)
{
    C_0083bc28_RefreshBeforeDispatch(self, nullptr);

    switch (self->field_004c)
    {
    case 3u:
        C_0083bc28_HandleStateThree(self, nullptr, a2);
        break;
    case 4u:
        C_0083bc28_HandleStateFour(self, nullptr, a2);
        break;
    case 5u:
    {
        float value;
        std::memcpy(&value, &a2, sizeof value);
        C_0083bc28_HandleStateFive(self, nullptr, value);
        break;
    }
    }
}

HOOK(0x4D4890, C_0083bc28_DispatchStateHandler, "auto");
