#ifndef DWARF_MATERIAL_H_
#define DWARF_MATERIAL_H_
#pragma once

#include <map>

#include "Color.h"

namespace Dwarf
{
    struct MaterialData
    {
        glm::vec3 Ka; // ambient color (values between 0 and 1)
        glm::vec3 Kd; // diffuse color (values between 0 and 1)
        glm::vec3 Ks; // specular color (values between 0 and 1)
        glm::vec3 Tf; // transmittance || transmission filter (values between 0 and 1)
        glm::vec3 Ke; // emission || emissive (values between 0 and 1)
        float Ns; // shininess || specular exponent (values between 0 and 1000)
        float Ni; // ior - refraction (values between 0.001 and 10)
        float d; // (or Tr where Tr = 1 - d) dissolve || transparency (values between 0 and 1 with 0.0 being fully dissolved - completely transparent and 1.0 fully opaque)
        //int illum; // illumination model
    };

	class Material
	{
	public:
        Material(size_t id);
		virtual ~Material();

        void setAmbient(const Color &color);
        void setDiffuse(const Color &color);
        void setSpecular(const Color &color);
        void setEmissive(const Color &color);
        void setTransmissionFilter(const Color &color);
        void setSpecularExponent(float value);
        void setRefraction(float value);
        void setDissolve(float value);
        size_t getID() const;

	private:
        const size_t _id;
        MaterialData _data;
	};
}

#endif // DWARF_MATERIAL_H_
