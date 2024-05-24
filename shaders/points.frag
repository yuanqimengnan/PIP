#version 430
out vec4 fragColor;
flat in int pointid;
layout(std430, binding = 2) buffer MyBuffer {
    int dynamic[];
};
uniform sampler2D PolyTex;
void main()
{
    vec4 pix = texelFetch(PolyTex, ivec2(gl_FragCoord.xy), 0);
    int r = int (ceil(pix.r));
    int b = int (ceil(pix.b));
    if (r!=0) {
        dynamic[pointid] = b;
    }
    fragColor = vec4(0,0,0,1);
}
