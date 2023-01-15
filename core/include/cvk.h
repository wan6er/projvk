#pragma once

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/surface.h"
#include "cvk/swapchain.h"
#include "cvk/memory.h"

#include "cvk/image/image2d.h"

#include "cvk/pipe/base_render_pass.h"

// #include "cvk/initialize/instance_initialize.h"
#include "cvk/initialize/device_initialize.h"
#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "cvk/initialize/memory_initialize.h"
#include "cvk/initialize/image_initialize.h"

#if defined(_WIN32)

#include "win32/surface_win32.h"
// #include "cvk/initialize/win32_initialize.h"

#endif