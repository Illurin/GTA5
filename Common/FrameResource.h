#pragma once
#include "vkUtil.h"

class FrameResource {
public:
	FrameResource(vk::Device* device, vk::PhysicalDeviceMemoryProperties gpuProp, uint32_t passCount, uint32_t objectCount, uint32_t materialCount, uint32_t skinnedObjectCount) {
		vk::MemoryPropertyFlags memProp = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
		vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eUniformBuffer;

		passCB.resize(passCount);
		for (uint32_t i = 0; i < passCount; i++)
			passCB[i] = std::make_unique<Buffer<PassConstants>>(device, 1, usage, gpuProp, memProp, true);

		objCB.resize(objectCount);
		for (uint32_t i = 0; i < objectCount; i++)
			objCB[i] = std::make_unique<Buffer<ObjectConstants>>(device, 1, usage, gpuProp, memProp, true);

		matCB.resize(materialCount);
		for (uint32_t i = 0; i < materialCount; i++)
			matCB[i] = std::make_unique<Buffer<MaterialConstants>>(device, 1, usage, gpuProp, memProp, true);

		skinnedCB.resize(skinnedObjectCount);
		for (uint32_t i = 0; i < skinnedObjectCount; i++)
			skinnedCB[i] = std::make_unique<Buffer<SkinnedConstants>>(device, 1, usage, gpuProp, memProp, true);
	}
	~FrameResource(){}

	std::vector<std::unique_ptr<Buffer<PassConstants>>> passCB;				   //ÿ֡��һ��Pass�����еĳ���
	std::vector<std::unique_ptr<Buffer<ObjectConstants>>> objCB;			   //������Ⱦ��ʹ�õĳ���
	std::vector<std::unique_ptr<Buffer<MaterialConstants>>> matCB;			   //ÿ��������ʹ�õĵĳ���
	std::vector<std::unique_ptr<Buffer<SkinnedConstants>>> skinnedCB;		   //ÿ����ɫ�Ǽܵ�ƫ����
};