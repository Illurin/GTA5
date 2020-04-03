#pragma once
#include "vkUtil.h"

class PostProcessingProfile {
public:
	struct Bloom {
		float criticalValue = 0.5f;
		float blurOffset = 0.003f;
		int blurRadius = 10;
	};

	struct HDR {
		float exposure = 1.0f;
	};
};

namespace PostProcessing {

	class Bloom {
	public:
		Bloom(PostProcessingProfile::Bloom& profile) {
			bloomProfile = profile;
		}
		~Bloom() {
			DestroyAttachment(vkInfo->device, renderTarget0);
			DestroyAttachment(vkInfo->device, renderTarget1);
			vkInfo->device.destroy(sampler);
			vkInfo->device.destroy(bloomRenderPass);
			vkInfo->device.destroy(combineRenderPass);
			for (auto& framebuffer : bloomFramebuffers) {
				vkInfo->device.destroy(framebuffer);
			}
			for (auto& framebuffer : combineFramebuffers) {
				vkInfo->device.destroy(framebuffer);
			}
			for (auto& layout : descSetLayout) {
				vkInfo->device.destroy(layout);
			}
			for (auto& layout : pipelineLayout) {
				vkInfo->device.destroy(layout);
			}
			for (auto& pipeline : pipelines) {
				vkInfo->device.destroy(pipeline.second);
			}
		}

		void SetHDRProperties(float exposure);

		vk::RenderPass GetRenderPass()const { return combineRenderPass; }

		void Begin(vk::CommandBuffer cmd, uint32_t currentImage);

		//���ⲿVulkan��Ϣ������
		Vulkan* vkInfo;

		void PrepareRenderPass();
		void PrepareFramebuffers();
		void PrepareDescriptorSets(vk::ImageView sourceImage);
		void PreparePipelines();

	private:
		PostProcessingProfile::Bloom bloomProfile;
		std::unique_ptr<Buffer<PostProcessingProfile::HDR>> hdrProperties;

		//������ȾĿ���Ӧ��Image��ImageView�Լ�Framebuffer
		Attachment renderTarget0;
		Attachment renderTarget1;

		std::vector<vk::Framebuffer> bloomFramebuffers;
		std::vector<vk::Framebuffer> combineFramebuffers;

		vk::Sampler sampler;

		//�洢�������ڼ����ͼƬ�Ͳ�������������
		std::vector<vk::DescriptorSet> descSets;
		std::vector<vk::DescriptorSetLayout> descSetLayout;
		std::vector<vk::PipelineLayout> pipelineLayout;

		//���й���
		std::unordered_map<std::string, vk::Pipeline> pipelines;

		//��Ⱦ����
		vk::RenderPass bloomRenderPass;
		vk::RenderPass combineRenderPass;
	};

}