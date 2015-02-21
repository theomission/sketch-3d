#include "render/OpenGL/BufferObjectOpenGL.h"

#include "math/Matrix4x4.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace Sketch3D {

BufferObjectOpenGL::BufferObjectOpenGL(const VertexAttributesMap_t& vertexAttributes, BufferUsage_t usage) : BufferObject(vertexAttributes, usage),
        vao_(0), vbo_(0), ibo_(0), instanceBuffer_(0)
{
}

BufferObjectOpenGL::~BufferObjectOpenGL() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
    
    if (instanceBuffer_ != 0) {
        glDeleteBuffers(1, &instanceBuffer_);
    }
}

void BufferObjectOpenGL::Render() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_SHORT, 0);
}

void BufferObjectOpenGL::RenderInstances(const vector<Matrix4x4>& modelMatrices) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * modelMatrices.size(), &modelMatrices[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(vao_);

    glDrawElementsInstanced(GL_TRIANGLES, indexCount_, GL_UNSIGNED_SHORT, 0, modelMatrices.size());
}

bool BufferObjectOpenGL::SetVertexData(const vector<float>& vertexData, int presentVertexAttributes) {
    // We implicitely count position
    size_t count = 1;
    VertexAttributesMap_t::iterator it = vertexAttributes_.begin();

    for (; it != vertexAttributes_.end(); ++it) {
        if ((presentVertexAttributes & it->first) > 0) {
            count += 1;
        }
    }

    if (count != vertexAttributes_.size()) {
        return false;
    }

    GenerateBuffers();

    // We want to allocate data for a new buffer if there's nothing in there or if the new data that we want to put in
    // the buffer is not of the same size as the old one
    if (vertexData.size() != vertexCount_) {
        vertexCount_ = vertexData.size();

        // We first bind the vertex array object nad then bind the two other buffers
        glBindVertexArray(vao_);

        // Vertex buffer object
        int type = (usage_ == BUFFER_USAGE_STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	    glBufferData(GL_ARRAY_BUFFER, vertexCount_ * sizeof(float), &vertexData[0], type);

        // Calculate offset and array index depending on vertex attributes provided by the user
        map<size_t, VertexAttributes_t> attributesFromIndex;
        VertexAttributesMap_t::iterator it = vertexAttributes_.begin();
        for (; it != vertexAttributes_.end(); ++it) {
            attributesFromIndex[it->second] = it->first;
        }

        bool hasNormals = ((presentVertexAttributes & VERTEX_ATTRIBUTES_NORMAL) > 0);
        bool hasTexCoords = ((presentVertexAttributes & VERTEX_ATTRIBUTES_TEX_COORDS) > 0);
        bool hasTangents = ((presentVertexAttributes & VERTEX_ATTRIBUTES_TANGENT) > 0);
        bool hasBones = ((presentVertexAttributes & VERTEX_ATTRIBUTES_BONES) > 0);
        bool hasWeights = ((presentVertexAttributes & VERTEX_ATTRIBUTES_WEIGHTS) > 0);

        size_t stride = sizeof(Vector3) +
                        ((hasNormals) ? sizeof(Vector3) : 0) +
                        ((hasTexCoords) ? sizeof(Vector2) : 0) +
                        ((hasTangents) ? sizeof(Vector3) : 0) +
                        ((hasBones) ? sizeof(Vector4) : 0) +
                        ((hasWeights) ? sizeof(Vector4) : 0);

        size_t cumulativeOffset = 0;
        map<size_t, VertexAttributes_t>::iterator v_it = attributesFromIndex.begin();
        for (; v_it != attributesFromIndex.end(); ++v_it) {
            size_t size = 0;
            size_t offset = 0;

            switch (v_it->second) {
                case VERTEX_ATTRIBUTES_POSITION:
                    size = 3;
                    offset = sizeof(Vector3);
                    break;

                case VERTEX_ATTRIBUTES_NORMAL:
                    if (!hasNormals) {
                        continue;
                    }

                    size = 3;
                    offset = sizeof(Vector3);
                    break;

                case VERTEX_ATTRIBUTES_TEX_COORDS:
                    if (!hasTexCoords) {
                        continue;
                    }

                    size = 2;
                    offset = sizeof(Vector2);
                    break;

                case VERTEX_ATTRIBUTES_TANGENT:
                    if (!hasTangents) {
                        continue;
                    }

                    size = 3;
                    offset = sizeof(Vector3);
                    break;

                case VERTEX_ATTRIBUTES_BONES:
                    if (!hasBones) {
                        continue;
                    }
                    
                    size = 4;
                    offset = sizeof(Vector4);
                    break;

                case VERTEX_ATTRIBUTES_WEIGHTS:
                    if (!hasWeights) {
                        continue;
                    }

                    size = 4;
                    offset = sizeof(Vector4);
                    break;
            }

            glEnableVertexAttribArray(v_it->first);
            glVertexAttribPointer(v_it->first, size, GL_FLOAT, GL_FALSE, stride, (void*)cumulativeOffset);
            cumulativeOffset += offset;
        }
    }

    // Otherwise, we want to simple change the data without reallocating everything
    else {
	    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(float), &vertexData[0]);
    }

    return true;
}

bool BufferObjectOpenGL::AppendVertexData(const vector<float>& vertexData, int presentVertexData) {
    return false;
}

void BufferObjectOpenGL::SetIndexData(unsigned short* indexData, size_t numIndex) {
    GenerateBuffers();

    indexCount_ = numIndex;

    glBindVertexArray(vao_);

    // Index buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount_ * sizeof(unsigned short), indexData, GL_STATIC_DRAW);
}

void BufferObjectOpenGL::AppendIndexData(unsigned short* indexData, size_t numIndex) {
}

void BufferObjectOpenGL::PrepareInstanceBuffers() {
    if (instanceBuffer_ != 0) {
        return;
    }

    glGenBuffers(1, &instanceBuffer_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);

    // Populate the buffer with the model matrices
    size_t attributeLocation = 0;
    VertexAttributesMap_t::iterator it = vertexAttributes_.begin();
    for (; it != vertexAttributes_.end(); ++it) {
        if (it->second > attributeLocation) {
            attributeLocation = it->second;
        }
    }
    attributeLocation += 1;

    for (size_t i = 0; i < 4; i++) {
        glEnableVertexAttribArray(attributeLocation + i);
        glVertexAttribPointer(attributeLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (const void*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(attributeLocation + i, 1);
    }
}

void BufferObjectOpenGL::GenerateBuffers() {
    if (vao_ == 0) {
        glGenVertexArrays(1, &vao_);
    }

    if (vbo_ == 0) {
        glGenBuffers(1, &vbo_);
    }

    if (ibo_ == 0) {
        glGenBuffers(1, &ibo_);
    }
}

}