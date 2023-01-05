#include "cvk/pipe/subpass.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

// ColorAttachment
auto ColorAttachment::color_attachments() const -> std::vector<VkAttachmentReference> CONST_REFERENCE
{
    return *this;
}

// DepthAttachment
void DepthAttachment::attach(VkAttachmentReference CONST_REFERENCE reference)
{
    _attached = true;
    _reference = reference;
}

auto DepthAttachment::depth_attachments() const -> VkAttachmentReference CONST_REFERENCE
{
    return _reference;
}

auto DepthAttachment::is_attached() const -> bool
{
    return _attached;    
}

// InputAttachment
auto InputAttachment::input_attachments() const -> std::vector<VkAttachmentReference> CONST_REFERENCE
{
    return *this;
}

// Subpass
Subpass::Subpass(VkPipelineBindPoint bind_point) :
    _bind_point(bind_point)
{
    __cvk::get_default_subpass_description(bind_point, {}, {}, nullptr, _description);
}

auto Subpass::color() -> ColorAttachment&
{
    return *this;
}

auto Subpass::depth() -> DepthAttachment&
{
    return *this;
}

auto Subpass::input() -> InputAttachment&
{
    return *this;
}

VkSubpassDescription CONST_REFERENCE Subpass::get_description()
{
    __cvk::get_default_subpass_description(_bind_point, color_attachments(), input_attachments(), (is_attached() ? &depth_attachments() : nullptr), _description);
    return _description;
}

Subpass::operator VkSubpassDescription CONST_REFERENCE()
{
    return get_description();
}

};