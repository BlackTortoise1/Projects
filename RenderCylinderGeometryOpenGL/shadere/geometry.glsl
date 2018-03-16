#version 330
layout (lines) in;
layout (triangle_strip) out;
layout (max_vertices = 256) out;

uniform mat4 vm, pm; 
//uniform vec3 control_p1, control_p2, control_p3, control_p4;
uniform int num_puncte_generate;
uniform int num_instante;
uniform float max_translate;
uniform float max_rotate;
uniform vec3[16] c;
uniform float translate;

in int instanta[2];

vec3 rotateY(vec3 punct, float u){
	float x = punct.x * cos(u) - punct.z *sin(u);
	float z = punct.x * sin(u) + punct.z *cos(u);
	return vec3(x, punct.y, z);
}

vec3 translateY(vec3 punct, float s){
	return vec3(punct.x , punct.y + s, punct.z);
}

vec3 bezier(float t, vec3 c0, vec3 c1, vec3 c2, vec3 c3){
	return c0 * (1 - t)*(1 - t)*(1 - t) + c1 * 3 * t * (1 - t) * (1 - t) + c2 * 3 * t * t * (1 - t) + c3 * t * t * t;
}

void main(){
	
	//TODO
	int i, j, k, l;
	vec3[8] cn;
	vec3 aux;
	float x = 0.0f, y = 0.0f, z = 0.0f, rotate = 0.0f , portion = 0.0f;
	float d = 0.0f;
	
	if(float(instanta[0])/float(num_instante) > 1.0f/3.0f)
		rotate = radians(120.0f);


	if(float(instanta[0])/float(num_instante) > 2.0f/3.0f)
		rotate = radians(240.0f);
	for(l = 0; l < 2 ; l++)
	{
		for(k = 0; k < 3 ; k++)
		{
			for(i = 0; i < 4; i++)
			{
				aux = bezier(float(instanta[0]) / float(num_instante), c[i * 4], c[i * 4 + 1], c[i * 4 + 2], c[i * 4 + 3]);
				aux = rotateY(aux, rotate);
				aux = translateY(aux, d);
				cn[i] = aux;

				aux = bezier(float(instanta[0] + 1) / float(num_instante),  c[i * 4], c[i * 4 + 1], c[i * 4 + 2], c[i * 4 + 3]);
				aux = rotateY(aux, rotate);
				aux = translateY(aux, d);
				cn[4 + i] = aux;
			}

			for(i = 0; i < num_puncte_generate; i++)
			{
				gl_Position = pm * vm * vec4(bezier(float(i) / float(num_puncte_generate), cn[0], cn[1], cn[2], cn[3]), 1); EmitVertex();
				gl_Position = pm * vm * vec4(bezier(float(i + 1) / float(num_puncte_generate), cn[0], cn[1], cn[2], cn[3]), 1); EmitVertex();
		
				gl_Position = pm * vm * vec4(bezier(float(i) / float(num_puncte_generate), cn[4], cn[5], cn[6], cn[7]), 1); EmitVertex();
				gl_Position = pm * vm * vec4(bezier(float(i + 1) / float(num_puncte_generate), cn[4], cn[5], cn[6], cn[7]), 1); EmitVertex();
					EndPrimitive();
			}
				//EndPrimitive();
				rotate += radians(120.0f);
		}
		d += 15.0f;
	}
}