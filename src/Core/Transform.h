#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <Common/uuid.h>
using namespace glm;

class Transform
{
public:
    static Transform fromWorldMat(const mat4 &m)
    {
        vec3 s;
        vec3 p;
        quat r;
        vec3 _skew;
        vec4 _perspective;
        decompose(m, s, r, p, _skew, _perspective);
        auto euler = eulerAngles(r);
        return Transform(p, euler, s);
    }
    Transform(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 rotation = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
        : id(uuid::generate_uuid_v4()), m_position(position), m_rotation(rotation), m_scale(scale)
    {
        computeMatrix();
    }

    void computeMatrix()
    {
        auto t = translate(mat4(1.0f), m_position);
        auto r = toMat4(quat(m_rotation));
        auto s = glm::scale(mat4(1.0f), m_scale);
        m_transform = t * r * s;
        m_transformInv = inverse(m_transform);
    }

    vec3 position() const
    {
        return m_position;
    }
    mat4 worldToLocal() const
    {
        return m_transformInv;
    }

    mat4 localToWorld() const
    {
        return m_transform;
    }

    vec3 rotation() const
    {
        return m_rotation;
    }

    void setPosition(const vec3 &pos)
    {
        this->m_position = pos;
        computeMatrix();
    }

    void setRot(const vec3 &rot)
    {
        m_rotation = rot;
        computeMatrix();
    }

public:
    std::string id;

private:
    vec3 m_position;
    vec3 m_rotation;
    vec3 m_scale;
    mat4 m_transform;
    mat4 m_transformInv;
};