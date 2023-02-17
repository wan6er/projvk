#include "cvk/initialize/shader_initialize.h"



namespace __cvk
{

CVK_API void get_default_shader_module_create_info(uint32_t size, void CONST_PTR code, VkShaderModuleCreateInfo& create_info) 
{
    CVK_ASSERT(code != nullptr);
    CVK_ASSERT(size > 0);
    CVK_ASSERT(size % 4 == 0);
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = static_cast<uint32_t CONST_PTR>(code);
}

CVK_API void get_default_shader_module_create_info(std::vector<char> CONST_REFERENCE code, VkShaderModuleCreateInfo& create_info) 
{
    CVK_ASSERT(code.size() > 0);
    CVK_ASSERT(code.size() % 4 == 0);
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = static_cast<uint32_t>(code.size());
    create_info.pCode = reinterpret_cast<uint32_t CONST_PTR>(code.data());
}

CVK_API VkResult create_shader_module(VkDevice device, VkShaderModuleCreateInfo CONST_REFERENCE create_info, VkShaderModule& shader)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateShaderModule(device, &create_info, nullptr, &shader);
}

CVK_API void destroy_shader_module(VkDevice device, VkShaderModule shader)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(shader != VK_NULL_HANDLE);
    vkDestroyShaderModule(device, shader, nullptr);
}
    
} // namespace __cvk
