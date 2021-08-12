//#include "Model.h"
//
//Model::Model(const char* path)
//{
//	std::ifstream objFile;
//	objFile.exceptions(std::ifstream::badbit);
//	try {
//		objFile.open(path);
//		std::string line;
//		while (std::getline(objFile, line)) {
//
//			if (line.length() <= 0) 
//				continue;
//
//			char lineHeader[128] = { 0 };
//			int res = sscanf_s(line.c_str(), "%s", lineHeader, (unsigned int)(sizeof(lineHeader) / sizeof(char)));
//			if (res == EOF) { break; }
//
//			if (strcmp(lineHeader, "v") == 0) {
//				glm::vec3 vertex;
//				sscanf_s(line.substr(1, line.length()).c_str(), "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
//				this->vertices.push_back(vertex);	
//			}
//			else if (strcmp(lineHeader, "f") == 0) {
//				int count = 0;
//				for (int t = 0; line[t] != '\0'; t++) {
//					if (line[t] == ' ') count++;
//				}
//				if (count == 3) {
//
//					unsigned int vertexIndex[3];
//					int matches = sscanf_s(
//						line.substr(1, line.length()).c_str(),
//						"%d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
//					
//					this->indices.push_back(vertexIndex[0] - 1);
//					this->indices.push_back(vertexIndex[1] - 1);
//					this->indices.push_back(vertexIndex[2] - 1);
//				}
//				if (count == 4) {
//
//					unsigned int vertexIndex[4];
//					int matches = sscanf_s(
//						line.substr(1, line.length()).c_str(),
//						"%d %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3]);
//					
//					this->indices.push_back(vertexIndex[0] - 1);
//					this->indices.push_back(vertexIndex[1] - 1);
//					this->indices.push_back(vertexIndex[2] - 1);
//
//					this->indices.push_back(vertexIndex[0] - 1);
//					this->indices.push_back(vertexIndex[2] - 1);
//					this->indices.push_back(vertexIndex[3] - 1);
//				}
//			}
//		}
//
//		objFile.close();
//	}
//	catch (std::ifstream::failure e) {
//		logger->Err("Can't read file: %s", e.what());
//	}
//}
//
//std::vector<glm::vec3> Model::get_vertices()
//{
//	return this->vertices;
//}
//
//std::vector<unsigned int> Model::get_indices()
//{
//	return this->indices;
//}
