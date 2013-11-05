#ifndef _LUCID_MESH_LOADER_
#define _LUCID_MESH_LOADER_

#include "mesh.hpp"


class MeshLoader {

public:
	static Mesh* loadOBJ (const char*);
};

#endif