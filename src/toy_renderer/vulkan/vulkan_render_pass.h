#pragma once

#include <toy_renderer/api/api_render_pass.h>

#include <toy_renderer/handle.h>
#include <toy_renderer/render_pass_command_recorder_options.h>
#include <toy_renderer/utils/hash_utils.h>

#include <vulkan/vulkan.h>

namespace ToyRenderer {

struct VulkanRenderPassKeyColorAttachment {
    explicit VulkanRenderPassKeyColorAttachment(const ColorAttachment &attachment)
        : loadOperation(attachment.loadOperation)
        , storeOperation(attachment.storeOperation)
        , initialLayout(attachment.initialLayout)
        , finalLayout(attachment.finalLayout)
    {
    }

    bool operator==(const VulkanRenderPassKeyColorAttachment &other) const noexcept
    {
        // clang-format off
        return loadOperation == other.loadOperation
            && storeOperation == other.storeOperation
            && initialLayout == other.initialLayout
            && finalLayout == other.finalLayout;
        // clang-format on
    }

    bool operator!=(const VulkanRenderPassKeyColorAttachment &other) const noexcept
    {
        return !(*this == other);
    }

    AttachmentLoadOperation loadOperation{ AttachmentLoadOperation::Clear };
    AttachmentStoreOperation storeOperation{ AttachmentStoreOperation::Store };
    TextureLayout initialLayout{ TextureLayout::ColorAttachmentOptimal };
    TextureLayout finalLayout{ TextureLayout::ColorAttachmentOptimal };
};

struct VulkanRenderPassKeyDepthStencilAttachment {
    explicit VulkanRenderPassKeyDepthStencilAttachment(const DepthStencilAttachment &attachment)
        : depthLoadOperation(attachment.depthLoadOperation)
        , depthStoreOperation(attachment.depthStoreOperation)
        , stencilLoadOperation(attachment.stencilLoadOperation)
        , stencilStoreOperation(attachment.stencilStoreOperation)
        , initialLayout(attachment.initialLayout)
        , finalLayout(attachment.finalLayout)
    {
    }

    bool operator==(const VulkanRenderPassKeyDepthStencilAttachment &other) const noexcept
    {
        // clang-format off
        return depthLoadOperation == other.depthLoadOperation
            && depthStoreOperation == other.depthStoreOperation
            && stencilLoadOperation == other.stencilLoadOperation
            && stencilStoreOperation == other.stencilStoreOperation
            && initialLayout == other.initialLayout
            && finalLayout == other.finalLayout;
        // clang-format on
    }

    bool operator!=(const VulkanRenderPassKeyDepthStencilAttachment &other) const noexcept
    {
        return !(*this == other);
    }

    AttachmentLoadOperation depthLoadOperation{ AttachmentLoadOperation::Clear };
    AttachmentStoreOperation depthStoreOperation{ AttachmentStoreOperation::Store };
    AttachmentLoadOperation stencilLoadOperation{ AttachmentLoadOperation::Clear };
    AttachmentStoreOperation stencilStoreOperation{ AttachmentStoreOperation::Store };
    TextureLayout initialLayout{ TextureLayout::DepthStencilAttachmentOptimal };
    TextureLayout finalLayout{ TextureLayout::DepthStencilAttachmentOptimal };
};

struct VulkanRenderPassKey {
    explicit VulkanRenderPassKey(const RenderPassCommandRecorderOptions &options)
        : depthStencilAttachment(options.depthStencilAttachment)
    {
        uint32_t colorAttachmentCount = static_cast<uint32_t>(options.colorAttachments.size());
        colorAttachments.reserve(colorAttachmentCount);
        for (uint32_t i = 0; i < colorAttachmentCount; ++i)
            colorAttachments.emplace_back(VulkanRenderPassKeyColorAttachment{ options.colorAttachments.at(i) });
    }

    bool operator==(const VulkanRenderPassKey &other) const noexcept
    {
        if (depthStencilAttachment != other.depthStencilAttachment)
            return false;

        if (colorAttachments.size() != other.colorAttachments.size())
            return false;

        const uint32_t colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
        for (uint32_t i = 0; i < colorAttachmentCount; ++i) {
            if (colorAttachments.at(i) != other.colorAttachments.at(i))
                return false;
        }

        return true;
    }

    std::vector<VulkanRenderPassKeyColorAttachment> colorAttachments;
    VulkanRenderPassKeyDepthStencilAttachment depthStencilAttachment;
};

class VulkanResourceManager;

struct Device_t;

struct VulkanRenderPass : public ApiRenderPass {
    explicit VulkanRenderPass(VkRenderPass _renderPass,
                              VulkanResourceManager *_vulkanResourceManager,
                              Handle<Device_t> _deviceHandle);

    VkRenderPass renderPass{ VK_NULL_HANDLE };
    VulkanResourceManager *vulkanResourceManager{ nullptr };
    Handle<Device_t> deviceHandle;
};

} // namespace ToyRenderer

namespace std {

template<>
struct hash<ToyRenderer::VulkanRenderPassKey> {
    size_t operator()(const ToyRenderer::VulkanRenderPassKey &key) const
    {
        uint64_t hash = 0;

        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.depthLoadOperation);
        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.depthStoreOperation);
        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.stencilLoadOperation);
        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.stencilStoreOperation);
        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.initialLayout);
        ToyRenderer::hash_combine(hash, key.depthStencilAttachment.finalLayout);

        const uint32_t colorAttachmentCount = static_cast<uint32_t>(key.colorAttachments.size());
        for (uint32_t i = 0; i < colorAttachmentCount; ++i) {
            const auto &attachment = key.colorAttachments.at(i);
            ToyRenderer::hash_combine(hash, attachment.loadOperation);
            ToyRenderer::hash_combine(hash, attachment.storeOperation);
            ToyRenderer::hash_combine(hash, attachment.initialLayout);
            ToyRenderer::hash_combine(hash, attachment.finalLayout);
        }

        return hash;
    }
};

} // namespace std
