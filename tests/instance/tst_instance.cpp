#include <KDGpu/device.h>
#include <KDGpu/instance.h>
#include <KDGpu/vulkan/vulkan_graphics_api.h>

#include <KDGpu_KDGui/view.h>

#include <KDGui/gui_application.h>

#include <set>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

using namespace KDGui;
using namespace KDGpu;
using namespace KDGpuKDGui;

TEST_SUITE("Instance")
{
    TEST_CASE("Vulkan")
    {
        // GIVEN
        std::unique_ptr<GraphicsApi> api = std::make_unique<VulkanGraphicsApi>();

        // WHEN
        Instance instance = api->createInstance(InstanceOptions{
                .applicationName = "instance",
                .applicationVersion = SERENITY_MAKE_API_VERSION(0, 1, 0, 0) });

        // THEN
        CHECK(instance.isValid());

        SUBCASE("Has Adapters")
        {
            // WHEN
            const auto adapters = instance.adapters();

            // THEN
            CHECK(!adapters.empty());
        }

        SUBCASE("Can query instance extensions")
        {
            // WHEN
            const auto extensions = instance.extensions();

            // THEN
            // In theory this could be empty if a driver really doesn't have any extensions
            CHECK(!extensions.empty());
        }

        SUBCASE("Can create Device")
        {
            // WHEN
            Adapter *discreteGPUAdapter = instance.selectAdapter(AdapterDeviceType::Default);

            // THEN
            CHECK(discreteGPUAdapter);
            CHECK(discreteGPUAdapter->isValid());

            // WHEN
            Device device = discreteGPUAdapter->createDevice();

            // THEN
            CHECK(device.isValid());
        }

        SUBCASE("Can create Surface")
        {
            // GIVEN
            GuiApplication app;
            View v;

            // WHEN
            const SurfaceOptions surfaceOptions = View::surfaceOptions(&v);
            Surface s = instance.createSurface(surfaceOptions);

            // THEN
            CHECK(s.isValid());
        }

        SUBCASE("Can create Default Device and Adapter")
        {
            // GIVEN
            GuiApplication app;
            View v;
            const SurfaceOptions surfaceOptions = View::surfaceOptions(&v);
            Surface s = instance.createSurface(surfaceOptions);

            // WHEN
            AdapterAndDevice aAndD = instance.createDefaultDevice(s);

            // THEN
            CHECK(aAndD.adapter);
            CHECK(aAndD.adapter->isValid());
            CHECK(aAndD.device.isValid());
        }
    }

    TEST_CASE("Destruction")
    {
        // GIVEN
        std::unique_ptr<GraphicsApi> api = std::make_unique<VulkanGraphicsApi>();

        const InstanceOptions options{
            .applicationName = "instance",
            .applicationVersion = SERENITY_MAKE_API_VERSION(0, 1, 0, 0)
        };

        Handle<Instance_t> handle;

        SUBCASE("Going Out Of Scope")
        {
            {
                // WHEN
                Instance instance = api->createInstance(options);
                handle = instance.handle();

                // THEN
                CHECK(instance.isValid());
                CHECK(handle.isValid());
                CHECK(api->resourceManager()->getInstance(handle) != nullptr);
            }

            // THEN
            CHECK(api->resourceManager()->getInstance(handle) == nullptr);
        }

        SUBCASE("Move assigment")
        {
            // WHEN
            Instance instance = api->createInstance(options);
            handle = instance.handle();

            // THEN
            CHECK(instance.isValid());
            CHECK(handle.isValid());
            CHECK(api->resourceManager()->getInstance(handle) != nullptr);

            // WHEN
            instance = {};

            // THEN
            CHECK(api->resourceManager()->getInstance(handle) == nullptr);
        }
    }
}
