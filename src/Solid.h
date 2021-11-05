// Base solids class
// Written by Dr. Sergey G. Kosov in 2019 for Project X 
#pragma once

#include "PrimTriangle.h"
#include <fstream> 

class CSolid {
public:
	/**
	 * @brief Default constructor
	 */
	CSolid(void) {}
	/**
	 * @brief Constructor
	 * @details Loads the primitives from an .obj file and adds them to the scene
	 * @param pShader Pointer to the shader to be use with the parsed object
	 * @param fileName The full path to the .obj file
	 */
	CSolid(ptr_shader_t pShader, const std::string& fileName)
	{
		std::ifstream file(fileName);

		if (file.is_open()) {
			std::cout << "Parsing OBJFile : " << fileName << std::endl;

			std::vector<Vec3f> vVertexes;
			std::vector<Vec3f> vNormals;
			std::vector<Vec2f> vTextures;

			std::string line;

			int nFaces = 0;
			for (;;) {
				if (!getline(file, line)) break;
				std::stringstream ss(line);
				getline(ss, line, ' ');
				if (line == "v") {
					Vec3f v;
					for (int i = 0; i < 3; i++) ss >> v.val[i];
					// std::cout << "Vertex: " << v << std::endl;
					vVertexes.push_back(v);
				}
				else if (line == "vt") {
					Vec2f vt;
					for (int i = 0; i < 2; i++) ss >> vt.val[i];
					vt.val[1] = 1.0f - vt.val[1];
					vTextures.push_back(vt);
				}
				else if (line == "vn") {
					Vec3f vn;
					for (int i = 0; i < 3; i++) ss >> vn.val[i];
					vNormals.push_back(vn);
				}
				else if (line == "f") {
					nFaces++;
					//if (nFaces > 10000) continue;
					int v, n, t;
					Vec3i V, N, T;
					for (int i = 0; i < 3; i++) {
						getline(ss, line, ' ');
						sscanf(line.c_str(), "%d/%d/%d", &v, &t, &n);
						V.val[i] = v - 1;
						T.val[i] = t - 1;
						N.val[i] = n - 1;
					}
					//std::cout << "Face: " << V << std::endl;
					//std::cout << "Normal: " << N << std::endl;
					add(std::make_shared<CPrimTriangle>(pShader, vVertexes[V.val[0]], vVertexes[V.val[1]], vVertexes[V.val[2]],
						vTextures[T.val[0]], vTextures[T.val[1]], vTextures[T.val[2]],
						vNormals[N.val[0]], vNormals[N.val[1]], vNormals[N.val[2]]));
				}
				else if (line == "#") {}
				else {
					std::cout << "Unknown key [" << line << "] met in the OBJ file" << std::endl;
				}
			}

			file.close();
			std::cout << "Finished Parsing" << std::endl;
		}
		else
			std::cout << "ERROR: Can't open OBJFile " << fileName << std::endl;
	}
	CSolid(const CSolid&) = delete;
	virtual ~CSolid(void) = default;
	CSolid& operator=(const CSolid&) = delete;

	const std::vector<ptr_prim_t>&  getPrims(void) const { return m_vpPrims; }


protected:
	void add(const ptr_prim_t pPrim) { m_vpPrims.push_back(pPrim); }
	void add(const std::shared_ptr<CSolid> pSolid) {
		for (const auto& pPrim : pSolid->getPrims())
			m_vpPrims.push_back(pPrim);
	}


private:
	std::vector<ptr_prim_t>	m_vpPrims;
};
