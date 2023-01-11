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

} // namespace cvk
