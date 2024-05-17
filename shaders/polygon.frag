#version 430

out vec4 fragColor;
in float col;
//layout(std430, binding = 2) buffer MyBuffer {
//    int dynamic;
//};
void main()
{
    fragColor = vec4(1,0,col,1);
//    dynamic=int(col);
}
