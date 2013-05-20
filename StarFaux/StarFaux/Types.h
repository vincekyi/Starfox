#ifndef _TYPES_H_
#define _TYPES_H_

enum ShadingType { NONE, FLAT, GOURAUD, PHONG };

enum DrawType { MESH, FILLED };

enum TextureSamplingType { NEAREST_NEIGHBOR, BILINEAR, TRILINEAR };

enum TextureWrappingType { CLAMP, REPEAT };

#endif
