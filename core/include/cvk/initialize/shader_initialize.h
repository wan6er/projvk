#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    
CVK_API void get_default_shader_module_create_info(uint32_t size, void CONST_PTR code, VkShaderModuleCreateInfo& create_info);
CVK_API void get_default_shader_module_create_info(std::vector<char> CONST_REFERENCE code, VkShaderModuleCreateInfo& create_info);
CVK_API VkResult create_shader_module(VkDevice device, VkShaderModuleCreateInfo CONST_REFERENCE create_info, VkShaderModule& shader);
CVK_API void destroy_shader_module(VkDevice device, VkShaderModule shader);

} // namespace __cvk
