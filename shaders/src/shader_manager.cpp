#include "shader_manager.hpp"

#include "FRAG_SHADER.hpp"
#include "VERT_SHADER.hpp"

shaders::Res shaders::getVertexShader()
{
    return shaders::Res{ VERT_SHADER, VERT_SHADER_SIZE };
}

shaders::Res shaders::getFragmentShader()
{
    return shaders::Res{ FRAG_SHADER, FRAG_SHADER_SIZE };
}
