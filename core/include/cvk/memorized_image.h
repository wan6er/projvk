#pragma once

#include "cvk/vk_header.h"
#include "cvk/base/base_memorized.h"
#include "cvk/image/image2d.h"
#include "cvk/image/image_view.h"
#include "cvk/image/memorized_image_view.h"

namespace cvk
{
    
    typedef BaseMemorized<Image2D> MemorizedImage2D;
    typedef MemorizedImageView<ImageView2D> MemorizedImageView2D;

    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_IMAGE_TILING_OPTIMAL>>, MEMORY_STANDARD, VK_IMAGE_ASPECT_COLOR_BIT> StandardTexture2D;
    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_LINEAR>>, MEMORY_WRITABLE, VK_IMAGE_ASPECT_COLOR_BIT> WritableTexture2D;
    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_LINEAR>>, MEMORY_WRITABLE_FULL_SPEED, VK_IMAGE_ASPECT_COLOR_BIT> WritableTexture2DFullSpeed;

    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL>>, MEMORY_STANDARD, VK_IMAGE_ASPECT_COLOR_BIT> StandardColorAttachment2D;
    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_TILING_LINEAR>>, MEMORY_READABLE, VK_IMAGE_ASPECT_COLOR_BIT> ReadableColorAttachment2D;

    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL>>, MEMORY_STANDARD, VK_IMAGE_ASPECT_COLOR_BIT> StandardInputAttachment2D;
    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_TILING_LINEAR>>, MEMORY_READABLE, VK_IMAGE_ASPECT_COLOR_BIT> ReadableInputAttachment2D;

    typedef BaseTypeMemorizedImageView<ImageView<BaseTypeImage2D<VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL>>, MEMORY_STANDARD, VK_IMAGE_ASPECT_DEPTH_BIT> StandardDepthAttachment2D;
    
} // namespace cvk
