#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082ddf8_SyncAssociation, 0x0040EE80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsMatrixScalarNearZero, 0x006AE540, (S_006adb40* self, void* , int a2));

static char __fastcall C_0082ddf8_MatchesAssociatedObject(C_0082ddf8* self, void* /*edx*/, S_006adb40* a2)
{
    char result = 0;
    if (self->field_0018) {
        C_0082ddf8_SyncAssociation(self, nullptr);
        return (char)IsMatrixScalarNearZero(reinterpret_cast<S_006adb40*>(self->field_0020), nullptr, (int)a2);
    }
    return result;
}

HOOK(0x0040F0A0, C_0082ddf8_MatchesAssociatedObject, "auto");
