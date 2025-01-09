#include "MeshModel.h"

	MeshModel::MeshModel() {}
	void MeshModel::CreateMeshModel(Wavefront_obj& wf) {
		//Reset the mesh model
		Reset();
		//Extract Point
		for (Wavefront_obj::Vector& point : wf.m_points)
		{
			_points.emplace_back(point.e[0], point.e[1], point.e[2],1);
		} 
		//Extract Face Normals, Vertex Normals and Edges
		for (const Wavefront_obj::Face& face : wf.m_faces)
		{
			//Extract edges
			_edges.insert({ std::min(face.v[0],face.v[1]),std::max(face.v[0],face.v[1]) });
			_edges.insert({ std::min(face.v[1],face.v[2]),std::max(face.v[1],face.v[2]) });
			_edges.insert({ std::min(face.v[2],face.v[0]),std::max(face.v[2],face.v[0]) });

			//Extract normals for each face
			//calculate normal
			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(_points[face.v[1]] - _points[face.v[0]]), glm::vec3(_points[face.v[2]] - _points[face.v[0]])));
			//convert to vec4
			glm::vec4 normal4 = { normal.x,normal.y,normal.z,1 };
			//associate normal with relevant face
			std::array<int, 3> x = { face.v[0], face.v[1], face.v[2] };
			_faceNormals.push_back({ x,normal4 });

			//Associate face normals to each vertex - for each vertex in the face, add the faceNormal calculated to the list of normals (by index)
			_pointNormalsMap[face.v[0]].push_back(_faceNormals.size() - 1);
			_pointNormalsMap[face.v[1]].push_back(_faceNormals.size() - 1);
			_pointNormalsMap[face.v[2]].push_back(_faceNormals.size() - 1);
		}
		//Calculate vertex normals
		for (const auto& pointNormal : _pointNormalsMap)
		{
			glm::vec3 normal = { 0,0,0 };
			for (const int& faceIndex : pointNormal.second)
			{
				normal += glm::vec3(_faceNormals[faceIndex].second);
			}
			normal = glm::normalize(normal);
			_pointNormals[pointNormal.first] = { normal.x,normal.y,normal.z,1 };
		}

	}

	//Reset the mesh model
	void MeshModel::Reset() {
		_points.clear();
		_faceNormals.clear();
		_edges.clear();
		
	}


//wavefront -> BB -> center -> create obj vectors with homogenous coordinates