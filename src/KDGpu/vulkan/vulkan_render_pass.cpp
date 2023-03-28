#include "vulkan_render_pass.h"

namespace KDGpu {

VulkanRenderPass::VulkanRenderPass(VkRenderPass _renderPass,
                                   VulkanResourceManager *_vulkanResourceManager,
                                   Handle<Device_t> _deviceHandle)
    : ApiRenderPass()
    , renderPass(_renderPass)
    , vulkanResourceManager(_vulkanResourceManager)
    , deviceHandle(_deviceHandle)
{
}

} // namespace KDGpu