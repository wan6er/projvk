#include "cvk/shader.h"
#include "cvk/initialize/shader_initialize.h"

namespace cvk
{
    
Shader::Shader(uint32_t size, void CONST_PTR code)
{
    __cvk::get_default_shader_module_create_info(size, code, _create_info);
}

Shader::Shader(std::vector<char> CONST_REFERENCE code) :
    _code(code)
{
    __cvk::get_default_shader_module_create_info(_code, _create_info);
}

Shader::~Shader()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Shader::create(VkDevice device)
{
    _device = device;
    return __cvk::create_shader_module(_device, _create_info, object());
}

Shader::operator VkShaderModule CONST_REFERENCE () const
{
    return object();
}

void Shader::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_shader_module(_device, object());
    }
}

} // namespace cvk
