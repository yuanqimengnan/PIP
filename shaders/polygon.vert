#version 430
layout(location=0) in  vec2 vertex;
layout(location=1) in  float id;

out float col;

void main()
{
    float ndcX = vertex.x*2-1;
    float ndcY = vertex.y*2-1;
    gl_Position = vec4(ndcX,ndcY,0,1);
    col = id;
}
