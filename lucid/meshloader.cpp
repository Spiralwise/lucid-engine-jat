#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

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
	vector<float> normals;
	vector<short> indices;
	
	bool hasNormals = false;
	
	short index_counter = 0;
	map<string, short> optimized_index;
	string vertex_normal_index = "";
	char converted_number [8];
	int selected_index;
	
	vector<float> tmp_coord;
	vector<float> tmp_normals;
	float tmp_float [3];
	short tmp_short [6];
	
	char line_header [256];
	int code = fscanf (file, "%s", line_header);
	while ( code != EOF ) {
		
		// Vertices coordinates
		if ( !strcmp (line_header, "v") ) {
			fscanf (file, "%f %f %f\n", 
				tmp_float, 
				tmp_float+1, 
				tmp_float+2);
			tmp_coord.push_back (tmp_float[0]);
			tmp_coord.push_back (tmp_float[1]);
			tmp_coord.push_back (tmp_float[2]);
		}
		else if ( !strcmp (line_header, "vn" ) ) {
			hasNormals = true;
			fscanf (file, "%f %f %f\n",
				tmp_float, 
				tmp_float+1, 
				tmp_float+2);
			tmp_normals.push_back (tmp_float[0]);
			tmp_normals.push_back (tmp_float[1]);
			tmp_normals.push_back (tmp_float[2]);
		}
		// Indices
		else if ( !strcmp (line_header, "f") ) {
			if ( hasNormals ) {
				fscanf (file, "%d//%d %d//%d %d//%d\n", 
					tmp_short,
					tmp_short+1,
					tmp_short+2,
					tmp_short+3,
					tmp_short+4,
					tmp_short+5);
				for ( int i = 0; i < 3; i++ ) {
					itoa(tmp_short[i*2], converted_number, 10);
					vertex_normal_index = converted_number;
					vertex_normal_index += ".";
					itoa(tmp_short[i*2+1], converted_number, 10);
					vertex_normal_index += converted_number;
					if ( !optimized_index.count(vertex_normal_index) ) {
						optimized_index[vertex_normal_index] = index_counter++;
						
						selected_index = (tmp_short[i*2]-1)*3;
						coord.push_back ( tmp_coord[ selected_index ] );
						coord.push_back ( tmp_coord[ selected_index+1 ] );
						coord.push_back ( tmp_coord[ selected_index+2 ] );

						selected_index = (tmp_short[i*2+1]-1)*3;
						normals.push_back ( tmp_normals[ selected_index ] );
						normals.push_back ( tmp_normals[ selected_index+1 ] );
						normals.push_back ( tmp_normals[ selected_index+2 ] );
					}
					indices.push_back (optimized_index[vertex_normal_index]);
				}
			} else {
				fscanf (file, "%d %d %d\n", 
					tmp_short,
					tmp_short+1,
					tmp_short+2);
					for ( int i = 0; i < 3; i++ ) {
						coord.push_back (tmp_coord[tmp_short[i]-1]);
						indices.push_back (tmp_short[i]-1);
					}
			}
			/*indices.push_back (tmp_short[0]-1);
			indices.push_back (tmp_short[1]-1);
			indices.push_back (tmp_short[2]-1);*/
		}
		// Comment or empty line
		else {
			char dump_line[1000];
			fgets (dump_line, 1000, file);
		}

		code = fscanf (file, "%s", line_header);
	}
	
	if ( hasNormals ) {
		return new Mesh (coord.data(), normals.data(), coord.size(), indices.data(), indices.size());
	}
	else
		return new Mesh (coord.data(), coord.size(), indices.data(), indices.size());
}