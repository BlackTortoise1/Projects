//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"
//time, min
#include <ctime>
#include <algorithm>


class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int program_shader;														//id-ul de opengl al obiectului de tip program shader
	lab::Mesh** mesh;

	//informatii suprafata generate
	//glm::vec3 control_p1, control_p2, control_p3, control_p4;
	glm::vec3 controls[16];
	unsigned int num_puncte_generate, num_instante, counter;
	float max_translate, max_rotate, translate;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		mesh = new lab::Mesh*[6];
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		program_shader = lab::loadShader("shadere\\vertex.glsl","shadere\\geometry.glsl", "shadere\\fragment.glsl");
		
		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		view_matrix = glm::lookAt(glm::vec3(0,30,50), glm::vec3(0,10,0), glm::vec3(0,1,0));

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//creaza mesha
		counter = 0;
		num_puncte_generate = 10;
		num_instante = 20;
		max_translate = 80.0f;
		max_rotate = glm::radians(360.0f);
		mesh[0] = NULL;
		mesh[0] = generateMesh();
		
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shader
		glDeleteProgram(program_shader);

		//distruge mesh incarcat
		delete[] mesh;
	}

	glm::vec3 rotateY(glm::vec3 punct, float u) {
		float x = punct.x * cos(u) - punct.z *sin(u);
		float z = punct.x * sin(u) + punct.z *cos(u);
		return glm::vec3(x, punct.y, z);
	}
	//--------------------------------------------------------------------------------------------
	//functie de constructie mesha generata-------------------------------------------------------
	lab::Mesh* generateMesh(){
		//distruge daca exista
		//if (mesh) delete mesh;

		//defineste punctele de control

		float alfa = ((float)4 / (float)3) * tanf(glm::radians(30.0f));
		glm::vec3 p0, p1, p2, p3;
		p0 = glm::vec3(-20, 20, 0);
		p3 = rotateY(p0, glm::radians(120.0f));
		p1 = p0 + alfa * (rotateY(p0, glm::radians(90.0f)));
		p2 = p3 - alfa * (rotateY(p3, glm::radians(90.0f)));

		controls[0] = glm::vec3(p0.x, 20.0f, p0.z);
		controls[1] = glm::vec3(p1.x, 20.0f, p1.z);
		controls[2] = glm::vec3(p2.x, 20.0f, p2.z);
		controls[3] = glm::vec3(p3.x, 20.0f, p3.z);
		controls[4] = glm::vec3(p0.x, 15.0f, p0.z);
		controls[5] = glm::vec3(p1.x, 15.0f, p1.z);
		controls[6] = glm::vec3(p2.x, 15.0f, p2.z);
		controls[7] = glm::vec3(p3.x, 15.0f, p3.z);
		controls[8] = glm::vec3(p0.x, 10, p0.z);
		controls[9] = glm::vec3(p1.x, 10, p1.z);
		controls[10] = glm::vec3(p2.x, 10, p2.z);
		controls[11] = glm::vec3(p3.x, 10, p3.z);
		controls[12] = glm::vec3(p0.x, 5, p0.z);
		controls[13] = glm::vec3(p1.x, 5, p1.z);
		controls[14] = glm::vec3(p2.x, 5, p2.z);
		controls[15] = glm::vec3(p3.x, 5, p3.z);

		for (int i = 0; i < 16;i++)
		{
			printf("%f %f %f\n", controls[i].x, controls[i].y, controls[i].z);
		}

		//date mesh
		std::vector<lab::VertexFormat> vertices;
		vertices.push_back(lab::VertexFormat(-40, -25, 10));
		vertices.push_back(lab::VertexFormat(-40, 55, 10));
		std::vector<unsigned int> indices;
		indices.push_back(0);
		indices.push_back(1);

		//obiecte OpenGL mesh
		unsigned int vbo, ibo, vao;
		glGenVertexArrays(1 , &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(lab::VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lab::VertexFormat), (void*)0);
		return new lab::Mesh(vbo, ibo, vao, indices.size());
	}

	
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)

	void notifyBeginFrame(){
	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//foloseste shaderul
		glUseProgram(program_shader);
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(program_shader,"vm"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(program_shader,"pm"),1,false,glm::value_ptr(projection_matrix));
		glUniform3fv(glGetUniformLocation(program_shader,"c"), 16, glm::value_ptr(controls[0]));
		//glUniform3f(glGetUniformLocation(program_shader, "control_p1"), control_p2.x, control_p2.y, control_p2.z);
		//glUniform3f(glGetUniformLocation(program_shader,"control_p2"),control_p2.x, control_p2.y, control_p2.z);
		//glUniform3f(glGetUniformLocation(program_shader,"control_p3"),control_p3.x, control_p3.y, control_p3.z);
		//glUniform3f(glGetUniformLocation(program_shader,"control_p4"),control_p4.x, control_p4.y, control_p4.z);
		glUniform1i(glGetUniformLocation(program_shader,"num_puncte_generate"),num_puncte_generate);
		glUniform1i(glGetUniformLocation(program_shader, "ok"), 0);
		//glUniform1i(glGetUniformLocation(program_shader, "counter"), counter);
		glUniform1i(glGetUniformLocation(program_shader,"num_instante"),num_instante);
		glUniform1f(glGetUniformLocation(program_shader,"max_translate"),max_translate);
		glUniform1f(glGetUniformLocation(program_shader,"max_rotate"),max_rotate);
		//glUniform1f(glGetUniformLocation(program_shader, "rotate"), rotate);
		glUniform1f(glGetUniformLocation(program_shader, "translate"), translate);
		glProgramParameteriEXT(program_shader, GL_GEOMETRY_VERTICES_OUT_EXT, 1024);
		//mesh[0]->drawInstanced(num_instante);

		mesh[1] = mesh[0];
		for (int i = 1; i < 6; i++)
		{
			mesh[i] = mesh[0];
		}

		translate = 0;

		mesh[0]->drawInstanced(num_instante);
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		glViewport(0,0,width,height);
		float aspect = (float)width / (float)height;
		projection_matrix = glm::perspective(75.0f, aspect,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(program_shader);
			program_shader = lab::loadShader("shadere\\vertex.glsl", "shadere\\geometry.glsl", "shadere\\fragment.glsl");
		}
		if(key_pressed == '1'){
			if(num_puncte_generate < 10) num_puncte_generate++;
			std::cout<<"Numarul de puncte generate = "<<num_puncte_generate <<std::endl;
		}
		if(key_pressed == '2'){
			if (num_puncte_generate >3) num_puncte_generate--;
			std::cout<<"Numarul de puncte generate = "<<num_puncte_generate <<std::endl;
		}
		//glUniform1i(glGetUniformLocation(program_shader, "counter"), counter);
		//std::cout << "Numarul de puncte = " << counter << std::endl;
		if(key_pressed == '3'){
			if (num_instante < 200) num_instante++;
			std::cout << "Numarul de instante = " << num_instante << std::endl;
		}

		if(key_pressed == '4'){
			if (num_instante > 3) num_instante--;
			std::cout << "Numarul de instante = " << num_instante << std::endl;
		}
			
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("lab shadere 2 - suprafete de baleiere"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);
	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl;
	std::cout<<"\t1 ... creste numar puncte generate"<<std::endl<<"\t2 ... scade numar puncte generate"<<std::endl;
	std::cout<<"\t3 ... creste numar instante"<<std::endl<<"\t4 ... scade numar instante"<<std::endl;


	//run
	lab::glut::run();

	return 0;
}