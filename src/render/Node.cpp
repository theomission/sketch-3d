#include "render/Node.h"

#include "render/BufferObject.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/ModelManager.h"
#include "render/Renderer.h"
#include "render/RenderQueue.h"
#include "render/Shader.h"
#include "render/Texture2D.h"

#include <sstream>

namespace Sketch3D {

long long Node::nextNameIndex_ = 0;

Node::Node(Node* parent) : parent_(parent), mesh_(NULL), material_(NULL),
						   scale_(1.0f, 1.0f, 1.0)
{
    ostringstream convert;
    convert << nextNameIndex_;
	name_ = "NewNode" + convert.str();
	nextNameIndex_ += 1;
}

Node::Node(const string& name, Node* parent) : name_(name), parent_(parent),
											   mesh_(NULL), material_(NULL),
											   scale_(1.0f, 1.0f, 1.0f)
{
}

Node::Node(const Vector3& position, const Vector3& scale,
		   const Quaternion& orientation, Node* parent) : parent_(parent),
                                                          position_(position),
														  scale_(scale),
														  orientation_(orientation),
														  mesh_(NULL),
														  material_(NULL)
{
    ostringstream convert;
    convert << nextNameIndex_;
	name_ = "NewNode" + convert.str();
	nextNameIndex_ += 1;
}

Node::Node(const string& name, const Vector3& position, const Vector3& scale,
		   const Quaternion& orientation, Node* parent) : name_(name),
                                                          parent_(parent),
														  position_(position),
														  scale_(scale),
														  orientation_(orientation),
														  mesh_(NULL),
														  material_(NULL)
{
}

Node::Node(const Node& src) : parent_(src.parent_),
                              position_(src.position_),
                              scale_(src.scale_),
                              orientation_(src.orientation_),
                              material_(src.material_)
{
    // TODO
    // Better manage name copy
    name_ = src.name_ + "_c";

    // TODO
    // Better manager mesh and textures copy
    *mesh_  = *src.mesh_;
}

Node::~Node() {
}

void Node::Render(RenderQueue& renderQueue) const {
    renderQueue.AddItem(*this);

	map<string, Node*>::const_iterator it = children_.begin();
	for (; it != children_.end(); ++it) {
		it->second->Render(renderQueue);
	}
}

void Node::ImmediateRender() const {
	Shader* shader = material_->GetShader();
    shader->SetActive(true);

    const Matrix4x4& viewProjection = Renderer::GetInstance()->GetViewProjectionMatrix();
    Matrix4x4 model;

    // Setup the transformation matrix for this node
    model[0][0] = scale_.x;
    model[1][1] = scale_.y;
    model[2][2] = scale_.z;

    Matrix4x4 rotation;
    orientation_.ToRotationMatrix(rotation);
    model = rotation * model;

    model[0][3] = position_.x;
    model[1][3] = position_.y;
    model[2][3] = position_.z;

    Matrix4x4 modelViewProjection = viewProjection * model;
    Matrix4x4 modelView = Renderer::GetInstance()->GetViewMatrix() * model;
    shader->SetUniformMatrix4x4("modelViewProjection", modelViewProjection);
    shader->SetUniformMatrix4x4("modelView", modelView);
    shader->SetUniformMatrix4x4("view", Renderer::GetInstance()->GetViewMatrix());

    // Get the rendering data
    BufferObject** bufferObjects;
    vector<ModelSurface_t> surfaces;
    mesh_->GetRenderInfo(bufferObjects, surfaces);

    // TEMP
    // Material's textures
    const map<string, Texture*>& materialTextures = material_->GetTextures();
    map<string, Texture*>::const_iterator it = materialTextures.begin();

    for (; it != materialTextures.end(); ++it) {
        if (it->second != nullptr) {
            size_t textureUnit = it->second->Bind();
            shader->SetUniformTexture(it->first, textureUnit);
        }
    }

    // Render the mesh
    for (size_t i = 0; i < surfaces.size(); i++) {
        // Textures associated with the surface
        for (size_t j = 0; j < surfaces[i].geometry->numTextures; j++) {
            Texture2D* texture = surfaces[i].geometry->textures[j];
            if (texture != nullptr) {
                size_t textureUnit = texture->Bind();
                shader->SetUniformTexture("texture" + to_string(j), textureUnit);
            }
        }

        bufferObjects[i]->Render();
    }
}

bool Node::AddChildren(Node* node) {
	string name = node->GetName();
	map<string, Node*>::iterator it = children_.find(name);
	if (it != children_.end()) {
		return false;
	}

	children_[name] = node;
	return true;
}

Node* Node::GetNodeByName(const string& name) const {
    map<string, Node*>::const_iterator it = children_.find(name);
    if (it != children_.end()) {
        return it->second;
    }

    Node* node = nullptr;
    for (it = children_.begin(); it != children_.end(); ++it) {
        node = it->second->GetNodeByName(name);

        if (node != nullptr) {
            return node;
        }
    }

    return nullptr;
}

bool Node::RemoveChildrenByName(const string& name) {
	map<string, Node*>::iterator it = children_.find(name);
	if (it != children_.end()) {
	    children_.erase(it);
	    return true;
	}

    for (it = children_.begin(); it != children_.end(); ++it) {
        if (it->second->RemoveChildrenByName(name)) {
            return true;
        }
    }

    return false;
}

bool Node::RemoveChildren(const Node* const node) {
    return RemoveChildrenByName(node->GetName());
}

void Node::Translate(const Vector3& translation) {
	position_ += translation;
}

void Node::Scale(const Vector3& scale) {
	scale_.x *= scale.x;
	scale_.y *= scale.y;
	scale_.z *= scale.z;
}

void Node::Pitch(float angle) {
	RotateAroundAxis(angle, Vector3::RIGHT);
}

void Node::Yaw(float angle) {
	RotateAroundAxis(angle, Vector3::UP);
}

void Node::Roll(float angle) {
	RotateAroundAxis(angle, Vector3::LOOK);
}

void Node::RotateAroundAxis(float angle, const Vector3& axis) {
	Quaternion rot;
	rot.MakeFromAngleAxis(angle, axis);
	rot.Normalize();
	orientation_ *= rot;
}

void Node::SetParent(Node* parent) {
	parent_ = parent;
}

void Node::SetPosition(const Vector3& position) {
	position_ = position;
}

void Node::SetScale(const Vector3& scale) {
	scale_ = scale;
}

void Node::SetOrientation(const Quaternion& orientation) {
	orientation_ = orientation;
}

void Node::SetMesh(Mesh* mesh) {
	mesh_ = mesh;
}

void Node::SetMaterial(Material* material) {
	material_ = material;
}

const string& Node::GetName() const {
	return name_;
}

Node* Node::GetParent() const {
	return parent_;
}

const Vector3& Node::GetPosition() const {
	return position_;
}

const Vector3& Node::GetScale() const {
	return scale_;
}

const Quaternion& Node::GetOrientation() const {
	return orientation_;
}

Mesh* Node::GetMesh() const {
	return mesh_;
}

Material* Node::GetMaterial() const {
	return material_;
}

}
