#include "cgltf/gltf_node.h"
#include "glm/gtc/matrix_transform.hpp"

namespace cgltf
{

NodeTransform::NodeTransform() :
    translation(0.0),
    scale(1.0),
    rotation(0.0, glm::vec3(0.0))
{
}

glm::mat4 NodeTransform::to_matrix() const 
{
    return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

} // namespace cgltf
