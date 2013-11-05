#include <stdio.h>
#include <string.h>
#include <vector>

#include "meshloader.hpp"

using namespace std;


Mesh* MeshLoader::loadOBJ (const char* filename) {

	printf ("Loading %s as an OBJ...\n", filename);
	
	FILE* file = fopen (filename, "r");
	if ( file == NULL ) {
		printf ("Failed to open %s\n", filename);
		return (Mesh*)0;
	}
	
	vector<float> coord;
	vector<short> indices;
	
	float tmp_coord [3];
	short tmp_index [3];
	
	char line_header [256];
	int code = fscanf (file, "%s", line_header);
	while ( code != EOF ) {
		
		// Vertices coordinates
		if ( !strcmp (line_header, "v") ) {
			fscanf (file, "%f %f %f\n", tmp_coord, tmp_coord+1, tmp_coord+2);
			coord.push_back (tmp_coord[0]);
			coord.push_back (tmp_coord[1]);
			coord.push_back (tmp_coord[2]);
		}
		// Indices
		else if ( !strcmp (line_header, "f") ) {
			fscanf (file, "%d %d %d\n", 
				tmp_index,
				tmp_index+1,
				tmp_index+2);
			indices.push_back (tmp_index[0]-1);
			indices.push_back (tmp_index[1]-1);
			indices.push_back (tmp_index[2]-1);
		}
		// Comment or empty line
		else {
			char dump_line[1000];
			fgets (dump_line, 1000, file);
		}

		code = fscanf (file, "%s", line_header);
	}
	
	return new Mesh (coord.data(), coord.size(), indices.data(), indices.size());
}