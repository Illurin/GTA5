#pragma once
#include "GeometryGenerator.h"
#include "SkinnedData.h"
#include "FrameResource.h"
#include "Texture.h"
#include "ShadowMap.h"
#include "Camera.h"
#include "SkinnedModel.h"

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 eulerAngle = glm::vec3(0.0f);
	glm::vec3 localEulerAngle = glm::vec3(0.0f);
};

struct Material {
	std::string name;

	uint32_t matCBIndex;
	Texture* diffuse;

	glm::mat4x4 matTransform;
	glm::vec4 diffuseAlbedo;
	glm::vec3 fresnelR0;
	float roughness;

	bool dirtyFlag = true;
};

struct GameObject {
	std::string name;
	Transform transform;
	Material* material;
	uint32_t objCBIndex;

	bool dirtyFlag = true;
};

struct MeshRenderer {
	GameObject* gameObject;

	int baseVertexLocation;
	int startIndexLocation;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

struct SkinnedMeshRenderer {
	GameObject* gameObject;

	int baseVertexLocation;
	int startIndexLocation;

	std::vector<SkinnedVertex> vertices;
	std::vector<uint32_t> indices;
};

class Scene {
public:
	//Create����
	GameObject* CreatePlane(std::string name, float width, float depth, float texRepeatX, float texRepeatY);
	GameObject* CreateGeosphere(std::string name, float radius, uint32_t numSubdivison);
	Material* CreateMaterial(std::string name, Texture* diffuse, glm::mat4x4 matTransform, glm::vec4 diffuseAlbedo, glm::vec3 fresnelR0, float roughness);

	void BindMaterial(GameObject* gameObject, Material* material);
	void BindMaterial(std::string gameObject, std::string material);

	//������Ӱ����
	void SetAmbientLight(glm::vec3 strength);
	void SetDirectionalLight(glm::vec3 direction, glm::vec3 strength);
	void SetPointLight(glm::vec3 position, glm::vec3 strength, float fallOffStart, float fallOffEnd);
	void SetSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 strength, float fallOffStart, float fallOffEnd, float spotPower);
	void SetShadowMap(uint32_t width, uint32_t height, glm::vec3 lightDirection, float radius);

	void SetMainCamera(Camera* mainCamera);

	void UpdateObjectConstants();
	void UpdatePassConstants();
	void UpdateMaterialConstants();

	void SetupVertexBuffer();
	void SetupDescriptors();
	void PreparePipeline();

	void DrawObject(uint32_t currentBuffer);

	Vulkan* vkInfo;

private:
	uint32_t frameCount = 1;
	uint32_t passCountPerFrame = 2;
	uint32_t passCount = 0;
	uint32_t descCount = 0;

	std::unordered_map<std::string, GameObject> gameObjects;
	std::unordered_map<std::string, Material> materials;

	std::unique_ptr<Buffer<Vertex>> vertexBuffer;
	std::unique_ptr<Buffer<SkinnedVertex>> skinnedVertexBuffer;
	std::unique_ptr<Buffer<uint32_t>> indexBuffer;

	std::vector<std::unique_ptr<FrameResource>> frameResources;

	//�����
	std::vector<MeshRenderer> meshRenderers;
	std::vector<SkinnedMeshRenderer> skinnedMeshRenderers;
	std::vector<SkinnedModelInstance> skinnedModelInst;

	//��������
	//�ƹ�
	glm::vec3 ambientLight;
	Light directionalLight;
	Light pointLight;
	Light spotLight;
	
	//��Ӱ
	ShadowMap shadowMap;

	//���
	Camera* mainCamera;
};
