#version 330 core
in vec2 textureCoordinateInput;
out vec4 fragmentColor;

void main()
{
    vec2 chessboardPosition = floor(textureCoordinateInput/0.125f);
    float patternMask = mod(chessboardPosition.x + mod(chessboardPosition.y, 2.0f), 2.0f);

    fragmentColor = patternMask * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}