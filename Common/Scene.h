#pragma once
#include "AssetContent.h"
#include "GeometryGenerator.h"
#include "SkinnedData.h"
#include "FrameResource.h"
#include "ShadowMap.h"
#include "Camera.h"
#include "SkinnedModel.h"

class Scene {
public:
	//Create����
	GameObject* CreatePlane(std::string name, float width, float depth, float texRepeatX, float texRepeatY);
	GameObject* CreateGeosphere(std::string name, float radius, uint32_t numSubdivison);
	Material* CreateMaterial(std::string name, Texture* diffuse, glm::mat4x4 matTransform, glm::vec4 diffuseAlbedo, glm::vec3 fresnelR0, float roughness);

	GameObject* LoadModel(std::string name, std::string modelPath, glm::vec4 diffuseAlbedo, glm::vec3 fresnelR0, float roughness);

	void BindMaterial(GameObject* gameObject, Material* material);
	void BindMaterial(std::string gameObject, std::string material);

	//Get����
	GameObject* GetGameObject(std::string name);

	//������Ӱ����
	void SetAmbientLight(glm::vec3 strength);
	void SetDirectionalLight(glm::vec3 direction, glm::vec3 strength);
	void SetPointLight(glm::vec3 position, glm::vec3 strength, float fallOffStart, float fallOffEnd);
	void SetSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 strength, float fallOffStart, float fallOffEnd, float spotPower);
	void SetShadowMap(uint32_t width, uint32_t height, glm::vec3 lightDirection, float radius);

	void SetMainCamera(Camera* mainCamera);

	//��պ��趨
	void SetSkybox(std::wstring imagePath, float radius, uint32_t subdivision);

	void UpdateObjectConstants();
	void UpdatePassConstants();
	void UpdateMaterialConstants();

	void SetupVertexBuffer();
	void SetupDescriptors();
	void PreparePipeline();

	void DrawObject(uint32_t currentBuffer);

	Vulkan* vkInfo;

private:
	uint32_t passCount = 2;

	AssetContent* assetContent;

	std::unordered_map<std::string, GameObject> gameObjects;
	std::unordered_map<std::string, Material> materials;

	std::unique_ptr<Buffer<Vertex>> vertexBuffer;
	std::unique_ptr<Buffer<SkinnedVertex>> skinnedVertexBuffer;
	std::unique_ptr<Buffer<uint32_t>> indexBuffer;

	std::unique_ptr<FrameResource> frameResources;

	//Pass������
	vk::DescriptorSet scenePassDesc;
	vk::DescriptorSet shadowPassDesc;
	vk::DescriptorSet drawShadowDesc;

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

	//��պ�
	struct {
		bool use = false;
		Texture image;
		float radius;
		uint32_t subdivision;

		int startIndexLocation;
		int baseVertexLocation;
		int indexCount;

		vk::DescriptorSet descSet;
	}skybox;
};
