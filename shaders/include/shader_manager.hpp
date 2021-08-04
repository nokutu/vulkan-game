#pragma once

#include <iostream>

namespace shaders
{
struct Res
{
    const unsigned char* data;
    size_t size;
};

Res getVertexShader();

Res getFragmentShader();

}
