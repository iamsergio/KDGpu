#pragma once

#include <KDGpu/graphics_api.h>
#include <KDGpu/kdgpu_export.h>
#include <KDGpu/vulkan/vulkan_resource_manager.h>

#include <memory>

namespace KDGpu {

class KDGPU_EXPORT VulkanGraphicsApi : public GraphicsApi
{
public:
    VulkanGraphicsApi();
    ~VulkanGraphicsApi() final;

private:
    std::unique_ptr<VulkanResourceManager> m_vulkanResourceManager;
};

} // namespace KDGpu