#version 330

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_color;
layout(location = 1) in vec3 v_normal;

out vec3 frag_color;
out vec3 frag_position;
out vec3 frag_normal;
out float opacity;

uniform int Transiction;
uniform int Step;
uniform int RenderType;
uniform float MaxSteps;

uniform float time;
uniform float valA;
uniform float valB;

void main()
{
	opacity = 1;

	// cerul
	if(RenderType == 0)
	{
		vec3 Color1 = vec3(0, 0, 0.9);
		vec3 Color2 = vec3(0.4, 0, 0.2);
		vec3 Color3 = vec3(0, 0, 0.3);

		if(Transiction == 0)
			frag_color = Color1 + (Color2 - Color1)/MaxSteps * Step;
		if(Transiction == 1)
			frag_color = Color2 + (Color3 - Color2)/MaxSteps * Step;
		if(Transiction == 2)
			frag_color = Color3 + (Color1 - Color3)/MaxSteps * Step;
	}

	// pamant
	if(RenderType == 1)
	{
		if(Transiction == 0)
			frag_color = vec3(0, 0.6, 0) - vec3(0, 0.3, 0)/MaxSteps * Step;
		
		if(Transiction == 1)
			frag_color = vec3(0, 0.3, 0);
		
		if(Transiction == 2)
			frag_color = vec3(0, 0.3, 0) + vec3(0, 0.3, 0)/MaxSteps * Step;
	}

	// soseaua
	if(RenderType == 2)
	{
		if(Transiction == 0)
			frag_color = vec3(0.6, 0.6, 0.6) - vec3(0.3, 0.3, 0.3)/MaxSteps * Step;
		
		if(Transiction == 1)
			frag_color = vec3(0.3, 0.3, 0.3);
		
		if(Transiction == 2)
			frag_color = vec3(0.3, 0.3, 0.3) + vec3(0.3, 0.3, 0.3)/MaxSteps * Step;
	}

	// obstacole
	if(RenderType == 3)
		frag_color = vec3(0.9, 0, 0.2);

	// bordura
	if(RenderType == 4)
		frag_color = vec3(0.5, 0, 1) + vec3(0, v_normal.y/2, 0);

	// masina
	if(RenderType == 5)
		frag_color = vec3(0, 0.2, 0.5) + (v_normal.x + v_normal.y + v_normal.x)/10;
	
	// roti
	if(RenderType == 6)
		frag_color = vec3(0.05, 0.05, 0.05) + (v_normal.x + v_normal.y + v_normal.x)/20;
	if(RenderType == 8)
		frag_color = vec3(1.0, 1.0, 0.05) + (v_normal.x + v_normal.y + v_normal.x)/20;

	if(RenderType == 9)
		frag_color = vec3(0.9, 0.9, 0.9) + (v_normal.x + v_normal.y + v_normal.x)/20;
		
	if(RenderType == 10)
		frag_color = vec3(0.5, 0.2, 0.0) + (v_normal.x + v_normal.y + v_normal.x)/20;

	frag_position = v_position;
	frag_normal = v_normal;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

	//fumul
	if(RenderType == 7)
	{
		frag_color = vec3(0.7, 0.7, 0.7);
		frag_color -= vec3(0.6) * time;
		
		//gl_Position.z += sin(time / valB);
		gl_Position.x += (sin(time *10* valB) * (valA + 0.1))/50 * time;
		gl_Position.y += time * (valB + 0.1)/4;
	}

	

}
