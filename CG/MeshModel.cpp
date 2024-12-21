#include "MeshModel.h"

	MeshModel::MeshModel() {}
	MeshModel::MeshModel(Wavefront_obj& wf) {
		//Extract Point
		for (Wavefront_obj::Vector& point : wf.m_points)
		{
			/*
			glm::vec4 newPoint;
			newPoint.x = point.e[0];
			newPoint.y = point.e[1];
			newPoint.z = point.e[2];
			newPoint.w = 1;
			_points.push_back(newPoint);
			*/
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
			//associate normal with relevant face
			std::array<int, 3> x = { face.v[0], face.v[1], face.v[2] };
			_normals.push_back({ x,normal });			
		}		
	}


//wavefront -> BB -> center -> create obj vectors with homogenous coordinates