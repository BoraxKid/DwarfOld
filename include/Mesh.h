#ifndef MESH_H_
#define MESH_H_
#pragma once

#include <unordered_map>
#include <chrono>

#include "Tools.h"
#include "Submesh.h"
#include "MaterialManager.h"

namespace Dwarf
{
	class Renderer;

	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class Mesh
	{
	public:
		Mesh(const vk::Device &device, Dwarf::MaterialManager &materialManager, const std::string &meshFilename);
		virtual ~Mesh();

		void loadFromFile(Dwarf::MaterialManager &materialManager, const std::string &filename);
        void updateTransformationMatrix();
        std::vector<IBuildable *> getBuildables();
        void move(double x, double y, double z);
        void move(glm::dvec3 movement);
        void setPosition(double x, double y, double z);
        void setPosition(glm::dvec3 position);
        void scale(double x, double y, double z);
        void scale(glm::dvec3 scale);
        void setScale(double x, double y, double z);
        void setScale(glm::dvec3 scale);
        void rotate(double x, double y, double z);
        void rotate(glm::dvec3 rotation);
        void setRotation(double x, double y, double z);
        void setRotation(glm::dvec3 rotation);

	private:
		const vk::Device &_device;

        std::vector<Submesh> _submeshes;
        glm::dvec3 _position;
        glm::dvec3 _rotation;
        glm::dvec3 _scale;
        glm::dmat4 _positionMatrix;
        glm::dmat4 _rotationMatrix;
        glm::dmat4 _scaleMatrix;
        glm::dmat4 _preciseTransformationMatrix;
        glm::mat4 _transformationMatrix;
        bool _left;
        bool _right;
        bool _up;
        bool _down;
	};
}

#endif // MESH_H_