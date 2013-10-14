#include "Matrix3x3.h"

#include "Vector3.h"
#include "Constants.h"
#include <math.h>

namespace Sketch3D
{

const Matrix3x3 Matrix3x3::IDENTITY(1.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f);

Matrix3x3::Matrix3x3()
{
    for (int i = 0; i < 9; i++) {
        data_[i] = 0.0f;
    }
}

Matrix3x3::Matrix3x3(float m11, float m12, float m13,
                     float m21, float m22, float m23,
                     float m31, float m32, float m33)
{
    data_[0] = m11;
    data_[1] = m12;
    data_[2] = m13;

    data_[3] = m21;
    data_[4] = m22;
    data_[5] = m23;
    
    data_[6] = m31;
    data_[7] = m32;
    data_[8] = m33;
}

Matrix3x3::Matrix3x3(float* data)
{
    for (int i = 0; i < 9; i++) {
        data_[i] = data[i];
    }
}

Matrix3x3::Matrix3x3(const Matrix3x3& src)
{
    for (int i = 0; i < 9; i++) {
        data_[i] = src.data_[i];
    }
}

Matrix3x3 Matrix3x3::Transpose() const
{
    float data[9];
    int cnt = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            data[cnt++] = data_[j*3 + i];
        }
    }

    return Matrix3x3(data);
}

void Matrix3x3::RotationAroundX(float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);

    data_[4] =  cos;
    data_[5] =  sin;
    data_[7] = -sin;
    data_[8] =  cos;

    data_[0] = 1.0f;
    data_[1] = data_[2] = data_[3] = data_[6] = 0.0f;
}

void Matrix3x3::RotationAroundY(float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);

    data_[0] =  cos;
    data_[2] = -sin;
    data_[3] =  sin;
    data_[5] =  cos;

    data_[4] = data_[8] = 1.0f;
    data_[1] = data_[6] = data_[7] = 0.0f;
}

void Matrix3x3::RotationAroundZ(float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);

    data_[0] =  cos;
    data_[1] =  sin;
    data_[3] = -sin;
    data_[4] =  cos;

    data_[8] = 1.0f;
    data_[2] = data_[5] = data_[6] = data_[7] = 0.0f;
}

void Matrix3x3::RotateAroundAxis(const Vector3& axis, float angle)
{
    float cos = cosf(angle);
    float sin = sinf(angle);
    float sum = 1.0f - cos;

    data_[0] = axis.x * axis.x * sum + cos;
    data_[1] = axis.x * axis.y * sum - (axis.z * sin);
    data_[2] = axis.x * axis.z * sum + (axis.y * sin);

    data_[3] = axis.y * axis.x * sum + (axis.z * sin);
    data_[4] = axis.y * axis.y * sum + cos;
    data_[5] = axis.y * axis.z * sum - (axis.x * sin);

    data_[6] = axis.z * axis.x * sum - (axis.y * sin);
    data_[7] = axis.z * axis.y * sum + (axis.x * sin);
    data_[8] = axis.z * axis.z * sum + cos;
}

float Matrix3x3::operator()(int col, int row) const
{
    return data_[row*3 + col];
}

float& Matrix3x3::operator()(int col, int row)
{
    return data_[row*3 + col];
}

Matrix3x3 Matrix3x3::operator-() const
{
    float data[9];
    for (int i = 0; i < 9; i++) {
        data[i] = -data_[i];
    }

    return Matrix3x3(data);
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) const
{
    Matrix3x3 mat;

    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                mat.data_[i+j] += m.data_[j + k * 3] * data_[i + k];
            }
        }
    }

    return mat;
}

void Matrix3x3::operator*=(const Matrix3x3& m)
{
    Matrix3x3 mat;

    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                mat.data_[i+j] += m.data_[j + k * 3] * data_[i + k];
            }
        }
    }

    (*this) = mat;
}

bool Matrix3x3::operator==(const Matrix3x3& m) const
{
    for (int i = 0; i < 9; i++) {
        if (fabs(data_[i] - m.data_[i]) >= EPSILON) {
            return false;
        }
    }

    return true;
}

bool Matrix3x3::operator!=(const Matrix3x3& m) const
{
    for (int i = 0; i < 9; i++) {
        if (fabs(data_[i] - m.data_[i]) >= EPSILON) {
            return true;
        }
    }

    return false;
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& m)
{
    if ((*this) == m) {
        return *this;
    }

    for (int i = 0; i < 9; i++) {
        data_[i] = m.data_[i];
    }

    return *this;
}

}