uniform float near;
uniform float far;

varying float depth;

void main() {
	vec4 viewPos = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	depth = (viewPos.z - near) / far;
	
	gl_Position = viewPos;
}