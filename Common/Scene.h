#pragma once
#include "GeometryGenerator.h"
#include "SkinnedModel.h"
#include "SkinnedData.h"
#include "FrameResource.h"
#include "ShadowMap.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "PostProcessing.h"
#include "ImGUI.h"
#include "RenderEngine.h"

class Scene {
public:
	void AddGameObject(GameObject& gameObject, GameObject* parent);
	void AddMaterial(Material& material);

	void AddMeshRenderer(GameObject* gameObject, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	void AddSkinnedMeshRenderer(GameObject* gameObject, std::vector<SkinnedVertex>& vertices, std::vector<uint32_t>& indices);
	void AddParticleSystem(GameObject* particle, GameObject* subParticle, ParticleSystem::Property& property, ParticleSystem::Emitter& emitter, ParticleSystem::Texture& texture, ParticleSystem::SubParticle& subParticleProperty);
	void AddSkinnedModelInstance(SkinnedModelInstance& skinnedModelInst);

	//Get����
	GameObject* GetGameObject(std::string name);
	Material* GetMaterial(std::string name);

	//������Ӱ����
	void SetAmbientLight(glm::vec3 strength);
	void SetDirectionalLight(int index, glm::vec3 direction, glm::vec3 strength);
	void SetPointLight(int index, glm::vec3 position, glm::vec3 strength, float fallOffStart, float fallOffEnd);
	void SetSpotLight(int index, glm::vec3 position, glm::vec3 direction, glm::vec3 strength, float fallOffStart, float fallOffEnd, float spotPower);
	void SetShadowMap(uint32_t width, uint32_t height, glm::vec3 lightDirection, float radius);

	void SetMainCamera(Camera* mainCamera);

	//��պ��趨
	void SetSkybox(Texture image, float radius, uint32_t subdivision);

	//�����趨
	void SetHDRProperty(float exposure, float gamma);
	void SetBloomPostProcessing(PostProcessingProfile::Bloom& profile);

	//GUI�趨
	void PrepareImGUI();
	void UpdateImGUI(float deltaTime);

	void UpdateObjectConstants();
	void UpdatePassConstants();
	void UpdateMaterialConstants();
	void UpdateSkinnedModel(float deltaTime);
	void UpdateCPUParticleSystem(float deltaTime);

	void SetupRenderEngine();
	void SetupVertexBuffer();
	void SetupDescriptors();
	void PreparePipeline();
	void PrepareShaderModel();

	void DrawObject(vk::CommandBuffer cmd, uint32_t currentBuffer);

	//Get���������ڱ༭����
	std::vector<GameObject*> GetRootObjects()const { return rootObjects; }
	std::vector<Material*> GetAllMaterials() {
		std::vector<Material*> materials;

		for (auto& material : this->materials) {
			materials.push_back(&material.second);
		}

		return materials;
	}
	Light* GetLights() { return lights; }
	uint32_t GetObjectCount() { return gameObjects.size(); }

	Vulkan* vkInfo;

private:
	uint32_t passCount = 2;

	std::vector<GameObject*> rootObjects;

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

	//����
	std::vector<vk::Pipeline> meshPipeline;
	std::vector<vk::Pipeline> skinnedMeshPipeline;

	//�����
	std::vector<MeshRenderer> meshRenderers;
	std::vector<SkinnedMeshRenderer> skinnedMeshRenderers;
	std::vector<ParticleSystem> particleSystems;
	std::vector<SkinnedModelInstance> skinnedModelInst;

	std::vector<MeshRenderer*> shaderModel[(int)ShaderModel::shaderModelCount];
	std::vector<SkinnedMeshRenderer*> skinnedShaderModel[(int)ShaderModel::shaderModelCount];

	//��������
	//�ƹ�
	glm::vec3 ambientLight;
	Light lights[NUM_DIRECTIONAL_LIGHT + NUM_POINT_LIGHT + NUM_SPOT_LIGHT];

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

	//ImGUI
	ImGUI* imgui = nullptr;

	//��Ⱦ����
	RenderEngine renderEngine;

	//����
	std::unique_ptr<PostProcessing::Bloom> bloom;
};