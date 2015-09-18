#include "utils.hpp"

using namespace std;

/**
 * Put a file content in a string.
 * Useful for shader programs reading.
 */
string LUtils::readFile( const string &filename ) {
	string output = "";
	
	ifstream file(filename.c_str());
	string line;
	if ( file.is_open() ) {
		while ( file.good() ) {
			getline (file, line);
			output += line + "\n";
		}
		file.close();
	}
	
	return output;
}

/**
 * Load a DDS image.
 */
/*gli::texture2D LUtils::loadDDS( const string &filename ) {

	return Texture( gli::loadStorageDDS(filename) );
}*/