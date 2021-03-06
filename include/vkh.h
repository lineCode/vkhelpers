#ifndef VK_HELPERS_H
#define VK_HELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include <vulkan/vulkan.h>

#define FB_COLOR_FORMAT VK_FORMAT_B8G8R8A8_UNORM

#define VK_CHECK_RESULT(f) 																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        printf("Fatal : VkResult is %d in %s at line %d\n", res,  __FILE__, __LINE__); \
        assert(res == VK_SUCCESS);																		\
    }																									\
}

typedef struct _vkh_app_t*		VkhApp;
typedef struct _vkh_device_t*   VkhDevice;
typedef struct _vkh_image_t*    VkhImage;
typedef struct _vkh_buffer_t*   VkhBuffer;
typedef struct _vkh_queue_t*    VkhQueue;
//typedef struct _vkh_presenter_t*    VkhPresenter;

///////////////////
VkhApp              vkh_app_create      (const char* app_name, int ext_count, const char* extentions[]);
void                vkh_app_destroy     (VkhApp app);
VkInstance          vkh_app_get_inst    (VkhApp app);
VkPhysicalDevice    vkh_app_select_phy  (VkhApp app, VkPhysicalDeviceType preferedPhyType);

///////////////////////////////
VkhImage vkh_image_create       (VkhDevice pDev, VkFormat format, uint32_t width, uint32_t height, VkImageTiling tiling,
                                    VkMemoryPropertyFlags memprops,	VkImageUsageFlags usage);
VkhImage vkh_image_ms_create    (VkhDevice pDev, VkFormat format, VkSampleCountFlagBits num_samples, uint32_t width, uint32_t height,
                                    VkMemoryPropertyFlags memprops,	VkImageUsageFlags usage);
VkhImage vkh_tex2d_array_create (VkhDevice pDev, VkFormat format, uint32_t width, uint32_t height, uint32_t layers,
                                    VkMemoryPropertyFlags memprops, VkImageUsageFlags usage);
void vkh_image_create_descriptor(VkhImage img, VkImageViewType viewType, VkImageAspectFlags aspectFlags, VkFilter magFilter, VkFilter minFilter,
                                    VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
void vkh_image_create_view      (VkhImage img, VkImageViewType viewType, VkImageAspectFlags aspectFlags);
void vkh_image_create_sampler   (VkhImage img, VkFilter magFilter, VkFilter minFilter,
                                    VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
void vkh_image_set_layout       (VkCommandBuffer cmdBuff, VkhImage image, VkImageAspectFlags aspectMask,
                                    VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages);
void vkh_image_set_layout_subres(VkCommandBuffer cmdBuff, VkhImage image, VkImageSubresourceRange subresourceRange,
                                    VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages);
void vkh_image_destroy          (VkhImage img);
void* vkh_image_map             (VkhImage img);
void vkh_image_unmap            (VkhImage img);

VkImage                 vkh_image_get_vkimage   (VkhImage img);
VkImageView             vkh_image_get_view      (VkhImage img);
VkImageLayout           vkh_image_get_layout    (VkhImage img);
VkSampler               vkh_image_get_sampler   (VkhImage img);
VkDescriptorImageInfo   vkh_image_get_descriptor(VkhImage img, VkImageLayout imageLayout);
////////////////////////////////
VkhBuffer   vkh_buffer_create   (VkhDevice pDev, VkBufferUsageFlags usage,
                                    VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size);
void        vkh_buffer_destroy  (VkhBuffer buff);
VkResult    vkh_buffer_map      (VkhBuffer buff);
void        vkh_buffer_unmap    (VkhBuffer buff);
VkResult    vkh_buffer_bind     (VkhBuffer buff);

VkBuffer    vkh_buffer_get_vkbuffer         (VkhBuffer buff);
void*       vkh_buffer_get_mapped_pointer   (VkhBuffer buff);
///////////////////////////////

VkFence         vkh_fence_create			(VkDevice dev);
VkFence         vkh_fence_create_signaled	(VkDevice dev);
VkSemaphore     vkh_semaphore_create		(VkDevice dev);

VkCommandPool   vkh_cmd_pool_create (VkDevice dev, uint32_t qFamIndex, VkCommandPoolCreateFlags flags);
VkCommandBuffer vkh_cmd_buff_create (VkDevice dev, VkCommandPool cmdPool, VkCommandBufferLevel level);
void vkh_cmd_begin  (VkCommandBuffer cmdBuff, VkCommandBufferUsageFlags flags);
void vkh_cmd_end    (VkCommandBuffer cmdBuff);
void vkh_cmd_submit (VkQueue queue, VkCommandBuffer *pCmdBuff, VkFence fence);
void vkh_cmd_submit_with_semaphores(VkQueue queue, VkCommandBuffer *pCmdBuff, VkSemaphore waitSemaphore,
                                    VkSemaphore signalSemaphore, VkFence fence);

VkShaderModule vkh_load_module(VkDevice dev, const char* path);

bool        memory_type_from_properties(VkPhysicalDeviceMemoryProperties* memory_properties, uint32_t typeBits,
                                        VkFlags requirements_mask, uint32_t *typeIndex);
char *      read_spv(const char *filename, size_t *psize);
uint32_t*   readFile(uint32_t* length, const char* filename);

void dumpLayerExts ();

void        set_image_layout(VkCommandBuffer cmdBuff, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout,
                      VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages);
void        set_image_layout_subres(VkCommandBuffer cmdBuff, VkImage image, VkImageSubresourceRange subresourceRange, VkImageLayout old_image_layout,
                      VkImageLayout new_image_layout, VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages);
/////////////////////
VkhQueue    vkh_queue_create    (VkhDevice dev, uint32_t familyIndex, uint32_t qIndex, VkQueueFlags flags);
void        vkh_queue_destroy   (VkhQueue queue);
VkhQueue    vkh_queue_find      (VkhDevice dev, VkQueueFlags flags);
/////////////////////
#endif
