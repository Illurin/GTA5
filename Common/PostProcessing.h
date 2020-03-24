#pragma once
#include "vkUtil.h"

class PostProcessingProfile {
public:
	struct Bloom {
		float criticalValue = 0.5f;
		float blurOffset = 0.003f;
		int blurRadius = 10;
	};
};

namespace PostProcessing {

	class Bloom {
	public:
		Bloom(PostProcessingProfile::Bloom& profile) {
			bloomProfile = profile;
		}
		void Init(Vulkan* vkInfo, vk::ImageView renderTarget);
		//��ȡ����
		void ExtractionBrightness(vk::CommandBuffer* cmd);
		//����ģ��
		void BlurH(vk::CommandBuffer* cmd);
		//����ģ��
		void BlurV(vk::CommandBuffer* cmd);
		//��ȡ�洢Bloom�㷨�����ͼƬ
		vk::ImageView GetImageView()const {
			return renderView0;
		}
		//�ṩָ��ԭʼ��ɫ��ͼ��������
		vk::DescriptorSet GetSourceDescriptor()const {
			return descSets[0];
		}
		//��ȡ��Ҫ��������������Sampler��SampledImage��
		int GetRequiredDescCount()const {
			return 3;
		}

	private:
		//���ⲿVulkan��Ϣ������
		Vulkan* vkInfo;

		PostProcessingProfile::Bloom bloomProfile;

		//������ȾĿ���Ӧ��Image��ImageView�Լ�Framebuffer
		vk::Image renderTarget0;
		vk::Image renderTarget1;
		vk::DeviceMemory renderMemory0;
		vk::DeviceMemory renderMemory1;
		vk::ImageView renderView0;
		vk::ImageView renderView1;
		vk::Framebuffer framebuffer0;
		vk::Framebuffer framebuffer1;

		//�洢�������ڼ����ͼƬ�Ͳ�������������
		std::vector<vk::DescriptorSet> descSets;
		vk::DescriptorSetLayout descSetLayout;
		vk::PipelineLayout pipelineLayout;

		//���й���
		std::unordered_map<std::string, vk::Pipeline> pipelines;

		//��Ⱦ����
		vk::RenderPass renderPass;

		void PrepareRenderPass();
		void PrepareFramebuffers();
		void PrepareDescriptorSets(vk::ImageView renderTarget);
		void PreparePipelines();
	};

}