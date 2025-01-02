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
		//Extract Normals and Edges
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
			_normals.push_back({ x,normal4 });
		}
	}

	//Reset the mesh model
	void MeshModel::Reset() {
		_points.clear();
		_normals.clear();
		_edges.clear();
	}


//wavefront -> BB -> center -> create obj vectors with homogenous coordinates