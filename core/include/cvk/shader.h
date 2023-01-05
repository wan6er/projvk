#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{
    
    class CVK_API Shader : public utils::BaseObj<VkShaderModule>
    {
    public:
        Shader(uint32_t size, void CONST_PTR code);
        Shader(std::vector<char> CONST_REFERENCE code);
        virtual ~Shader();

        VkResult create(VkDevice device);

        operator VkShaderModule CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkShaderModuleCreateInfo _create_info = {};
        VkDevice _device = VK_NULL_HANDLE;
        std::vector<char> _code;

    };

} // namespace cvk
