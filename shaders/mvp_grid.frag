// Credit(s): https://stackoverflow.com/questions/72762825/to-what-values-are-glsl-variables-initialized-by-default

#version 300 es

precision highp float;

in highp vec3 vertexPosition;

out mediump vec4 fragColor;

const float squareSize = 1.0f;
const vec3 color_l1 = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    // calculate deriviatives
    // (must be done at the start before conditionals)
    float dXy = abs(dFdx(vertexPosition.z)) / 2.0;
    float dYy = abs(dFdy(vertexPosition.z)) / 2.0;
    float dXx = abs(dFdx(vertexPosition.x)) / 2.0;
    float dYx = abs(dFdy(vertexPosition.x)) / 2.0;

    // find and fill horizontal lines
    int roundPos = int(vertexPosition.z / squareSize);
    float remainder = vertexPosition.z - float(roundPos)*squareSize;
    float width = max(dYy, dXy) * 2.0;

    if (remainder <= width)
    {
        float diff = (width - remainder) / width;
        fragColor = vec4(color_l1, diff);
        return;
    }

    if (remainder >= (squareSize - width))
    {
        float diff = (remainder - squareSize + width) / width;
        fragColor = vec4(color_l1, diff);
        return;
    }

    // find and fill vertical lines
    roundPos = int(vertexPosition.x / squareSize);
    remainder = vertexPosition.x - float(roundPos)*squareSize;
    width = max(dYx, dXx) * 2.0;

    if (remainder <= width)
    {
        float diff = (width - remainder) / width;
        fragColor = vec4(color_l1, diff);
        return;
    }

    if (remainder >= (squareSize - width))
    {
        float diff = (remainder - squareSize + width) / width;
        fragColor = vec4(color_l1, diff);
        return;
    }

    discard;
}