/* Trivial bare-bone vertex shader */
//#version 150
//in vec2 vertex;
void main()
{
    vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_Position = pos;//vec4(vertex.x, vertex.y, 0.0, 1.0);
}
