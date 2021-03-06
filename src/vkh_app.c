#include "vkh_app.h"

#define ENGINE_NAME     "vkheplers"
#define ENGINE_VERSION  1

VkhApp vkh_app_create (const char* app_name, int ext_count, const char* extentions[]) {
    VkhApp app = (VkhApp)malloc(sizeof(vkh_app_t));

    VkApplicationInfo infos = { .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                .pApplicationName = app_name,
                                .applicationVersion = 1,
                                .pEngineName = ENGINE_NAME,
                                .engineVersion = ENGINE_VERSION,
                                .apiVersion = VK_API_VERSION_1_0};
#if DEBUG
    const uint32_t enabledLayersCount = 1;
    //const char* enabledLayers[] = {"VK_LAYER_LUNARG_core_validation"};
    const char* enabledLayers[] = {"VK_LAYER_LUNARG_standard_validation"};
#else
    const uint32_t enabledLayersCount = 0;
    const char* enabledLayers[] = NULL;
#endif

    VkInstanceCreateInfo inst_info = { .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                       .pApplicationInfo = &infos,
                                       .enabledExtensionCount = ext_count,
                                       .ppEnabledExtensionNames = extentions,
                                       .enabledLayerCount = enabledLayersCount,
                                       .ppEnabledLayerNames = enabledLayers };

    VK_CHECK_RESULT(vkCreateInstance (&inst_info, NULL, &app->inst));

    VK_CHECK_RESULT(vkEnumeratePhysicalDevices (app->inst, &app->phyCount, NULL));

    return app;
}

void vkh_app_destroy (VkhApp app){
    vkDestroyInstance (app->inst, NULL);
    free (app);
}

VkInstance vkh_app_get_inst (VkhApp app) {
    return app->inst;
}

VkPhysicalDevice vkh_app_select_phy (VkhApp app, VkPhysicalDeviceType preferedPhyType) {
    VkPhysicalDevice phys[app->phyCount];
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices (app->inst, &app->phyCount, &phys));

    if (app->phyCount == 1)
        return phys[0];

    for (int i=0; i<app->phyCount; i++){
        VkPhysicalDeviceProperties phy;
        vkGetPhysicalDeviceProperties (phys[i], &phy);
        if (phy.deviceType & preferedPhyType){
            printf ("GPU: %s  vulkan:%d.%d.%d driver:%d\n", phy.deviceName,
                    phy.apiVersion>>22, phy.apiVersion>>12&2048, phy.apiVersion&8191,
                    phy.driverVersion);
            return phys[i];
        }
    }
    fprintf (stderr, "No suitable GPU found\n");
    exit (-1);
}
