#include <toy_renderer/compute_pipeline.h>
#include <toy_renderer/compute_pipeline_options.h>
#include <toy_renderer/compute_pass_command_recorder.h>
#include <toy_renderer/command_recorder.h>
#include <toy_renderer/device.h>
#include <toy_renderer/queue.h>
#include <toy_renderer/instance.h>
#include <toy_renderer/vulkan/vulkan_graphics_api.h>

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

using namespace ToyRenderer;

namespace {
inline std::string assetPath()
{
#if defined(TOY_RENDERER_ASSET_PATH)
    return TOY_RENDERER_ASSET_PATH;
#else
    return "";
#endif
}
} // namespace

TEST_CASE("ComputePassCommandRecorder")
{
    // GIVEN
    std::unique_ptr<GraphicsApi> api = std::make_unique<VulkanGraphicsApi>();
    Instance instance = api->createInstance(InstanceOptions{
            .applicationName = "ComputePassCommandRecorder",
            .applicationVersion = SERENITY_MAKE_API_VERSION(0, 1, 0, 0) });

    Adapter computeAdapter;
    // Select Adapter that supports Compute
    for (const auto &adapter : instance.adapters()) {
        const auto &queueTypes = adapter.queueTypes();
        for (const auto &queueType : queueTypes) {
            const bool hasCompute = queueType.supportsFeature(QueueFlags(QueueFlagBits::ComputeBit));
            if (hasCompute) {
                computeAdapter = adapter;
                break;
            }
        }
    }
    REQUIRE(computeAdapter.isValid());

    Device device = computeAdapter.createDevice();

    Queue computeQueue;
    const auto &queues = device.queues();
    for (const auto &q : queues) {
        if (q.flags() | QueueFlags(QueueFlagBits::ComputeBit)) {
            computeQueue = q;
            break;
        }
    }

    const auto computeShaderPath = assetPath() + "/shaders/tests/compute_pipeline/empty_compute.comp.spv";
    auto computeShader = device.createShaderModule(ToyRenderer::readShaderFile(computeShaderPath));

    // THEN
    REQUIRE(device.isValid());
    REQUIRE(computeQueue.isValid());
    REQUIRE(computeShader.isValid());

    SUBCASE("Can't be default constructed")
    {
        // EXPECT
        REQUIRE(!std::is_default_constructible<ComputePassCommandRecorder>::value);
        REQUIRE(!std::is_trivially_default_constructible<ComputePassCommandRecorder>::value);
    }

    SUBCASE("A constructed ComputePassCommandRecorder from a Vulkan API")
    {

        // GIVEN
        const PipelineLayoutOptions pipelineLayoutOptions{};
        const PipelineLayout pipelineLayout = device.createPipelineLayout(pipelineLayoutOptions);
        const ComputePipelineOptions computePipelineOptions{
            .layout = pipelineLayout,
            .shaderStage = ComputeShaderStage{ .shaderModule = computeShader.handle() }
        };
        const ComputePipeline computePipeline = device.createComputePipeline(computePipelineOptions);

        // THEN
        CHECK(computePipeline.isValid());

        // WHEN
        const CommandRecorderOptions commandRecorderOptions{
            .queue = computeQueue
        };
        CommandRecorder commandRecorder = device.createCommandRecorder(commandRecorderOptions);

        // THEN
        CHECK(commandRecorder.isValid());

        // WHEN
        ComputePassCommandRecorder computeCommandRecorder = commandRecorder.beginComputePass();

        // THEN
        CHECK(computeCommandRecorder.isValid());
    }
}