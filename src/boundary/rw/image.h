#pragma once
#include "life.h"
namespace csf_rw {
// Conversions import mutable game fields. Native owning pointers never escape.
rw::Image* imageNative(Image*);
rw::Raster* rasterNative(Raster*);
Raster* rasterFacade(rw::Raster*);
void rasterExport(Raster*);
rw::Texture* textureNative(Texture*);
Texture* textureFacade(rw::Texture*);
// Export AFTER native ownership changes, before the next inward conversion.
void textureExport(Texture*);
int textureRelease(Texture*);
bool textureCanRelease(Texture*, int references=1);
bool imagesEmpty();
bool imageBoundaryStart();
void imageBoundaryStop();
// Streaming slots are NOT allocated to slice 3. These helpers prepare their
// later integration; ordinary texture streaming stores references, not pixels.
bool textureStreamWrite(Texture*, Stream*);
Texture* textureStreamRead(Stream*);
Texture* streamTextureCreate(rw::Texture*);
void* streamImageForget(Registry, void*);
void streamImageExport();
bool attachStreamImageLifetime(); // zero-storage native destructor observers
}
