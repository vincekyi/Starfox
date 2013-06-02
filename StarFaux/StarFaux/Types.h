#ifndef _TYPES_H_
#define _TYPES_H_

enum ShadingType { NONE, FLAT, GOURAUD, PHONG };

enum DrawType { MESH, FILLED };

enum TextureSamplingType { NEAREST_NEIGHBOR, BILINEAR, TRILINEAR };

enum TextureWrappingType { CLAMP, REPEAT };

enum KeyType {
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L,
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, 
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
};

enum BoundingType { BOUNDING_BOX, BOUNDING_SPHERE };
#endif
