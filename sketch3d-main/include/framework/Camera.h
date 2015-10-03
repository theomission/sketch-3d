#ifndef SKETCH_3D_CAMERA_H
#define SKETCH_3D_CAMERA_H

#include "system/Platform.h"

#include "math/Matrix4x4.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"

namespace Sketch3D {

enum ConstrainPlane_t
{
    XY_PLANE,
    XZ_PLANE,
    YZ_PLANE,
    FREE_CAMERA
};

class SKETCH_3D_API Camera
{
public:
    Camera();

    void LookAt(const Vector3& position, const Vector3& point, const Vector3& up);

    void Walk(float units);
    void Fly(float units);
    void Strafe(float units);

    void Pitch(float radianAngle);
    void Yaw(float radianAngle);
    void Roll(float radianAngle);

    void SetConstrainPlane(ConstrainPlane_t constrainPlane);
    void SetPosition(const Vector3& position);
    void SetOrientation(const Quaternion& orientation);

    const Matrix4x4& GetViewMatrix();
    const Vector3& GetPosition() const;
    const Quaternion& GetOrientation() const;

private:
    ConstrainPlane_t    m_ConstrainPlane;
    Vector3             m_Position;
    Quaternion          m_Orientation;
    bool                m_NeedViewMatrixUpdate;
    Matrix4x4           m_ViewMatrix;
};

}

#endif