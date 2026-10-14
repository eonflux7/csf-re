#pragma once
#include "image.h"
namespace csf_rw {
// Private conversions: never publish native owning objects to game callbacks.
rw::Frame* frameNative(Frame*);
void frameExport();
void objectFrameChanged(ObjectWithFrame*);
void objectExport();
void objectImport();
bool objectsEmpty();
bool worldHasCamera(rw::World*);
rw::Atomic* atomicNative(Atomic*);
rw::Geometry* geometryNative(Geometry*);
rw::Material* materialNative(Material*);
rw::Light* lightNative(Light*);
rw::Camera* cameraNative(Camera*);
rw::Clump* clumpNative(Clump*);
// Construction helpers for slice 6's stream boundary and NULL fixtures. These
// are not additional original-address slots. Inputs are owned by the caller.
Material* materialCreate();
int materialDestroy(Material*);
Geometry* geometryCreate(int vertices, int triangles, unsigned flags);
bool geometryAppendMaterial(Geometry*, Material*);
bool geometryAddMorphTargets(Geometry*, int count);
Clump* clumpCreate();
bool clumpAddAtomic(Clump*, Atomic*);
bool clumpAddLight(Clump*, Light*);
void clumpSetFrame(Clump*, Frame*);
}
