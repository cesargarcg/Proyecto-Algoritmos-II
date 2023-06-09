#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

struct usuario{
	string nombres;
	string apellidos;
	unsigned long cedula;
	string tipoUsuario;
	string email;
	string clave;
	usuario *proximo;
};

//"Users" es el nodo cabeza de la lista de usuarios
usuario *users = NULL; 
usuario *usuarioActual = NULL; //lo usamos para el usuario que esta en el inicio de sesion

//Agregamos cada usuario por orden de cedula
void agregarUsuario(usuario *&user, string nombres, string apellidos, unsigned long cedula, string tipoUsuario, string email, string clave){
	usuario *nuevoUsuario = new usuario();
	
    nuevoUsuario->nombres= nombres;
	nuevoUsuario->apellidos = apellidos;
	nuevoUsuario->cedula = cedula;
	nuevoUsuario->tipoUsuario = tipoUsuario;
	nuevoUsuario->email = email;
	nuevoUsuario->clave= clave;
	
	usuario *auxiliar1 = user;
	usuario *auxiliar2;
	
	while ((auxiliar1 != NULL) && (auxiliar1->cedula < cedula)){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->proximo;
	}
	
	if (user == auxiliar1){
		user = nuevoUsuario;
	}
	else{
		auxiliar2->proximo = nuevoUsuario;
	}
	
	nuevoUsuario->proximo = auxiliar1;
}

//Funcion que muestra la lista de usuarios de tipo estudiante para el profesor
void mostrarListaUsuarios (usuario *listaUsuarios){
	usuario *user = new usuario();
	user = listaUsuarios;
	int contadorUsuarios = 1;
	
	while (user != NULL){
		if(user->tipoUsuario == "E"){
			cout<<"Usuario "<<contadorUsuarios<<":"<<endl;
			cout<<"Nombres: "<<user->nombres<<endl;
			cout<<"Apellidos: "<<user->apellidos<<endl;
			cout<<"Cedula: "<<user->cedula<<endl;
			cout<<"Tipo de usuario: "<<user->tipoUsuario<<endl; fflush(stdin);
			cout<<"Correo electronico: "<<user->email<<"\n"<<endl;
		}
		contadorUsuarios++;
		user = user->proximo;
	}
}

//Funcion que nos verifica que exista una lista de usuarios
bool existeLista(usuario *listaUsuarios){
	if (listaUsuarios == NULL){
		return false;
	}
	else{
		return true;
	}
}

//Funcion que verifica que una cedula ingresada exista en la lista de usuarios
bool verificarCedula (usuario *listaUsuarios, unsigned long cedula){
	usuario *user = new usuario();
	user = listaUsuarios;
	bool existe = false;
	
	while (user != NULL){
		if (cedula == user->cedula){
			existe = true;
		}
		user = user->proximo;
	}
	
	return existe;
}

//Funcion que verifica que la clave ingresada sea la que esta registrada en el nodo del usuario con la cedula ingresada
bool verificarClave (usuario *listaUsuarios, unsigned long cedula, string clave){
	usuario *user = new usuario();
	user = listaUsuarios;
	int encontroCedula = 0;
	bool correcta;
	
	while ((user != NULL) && (encontroCedula == 0)){
		if (cedula == user->cedula){
			encontroCedula = 1;
			if (clave == user->clave){
				correcta = true;
			}
			else{
				correcta = false;
			}
		}
		user = user->proximo;
	}
	return correcta;
}

//Funcion que nos verifica si ya existe un docente registrado para que, al momento de que un usuario se este registrando, no pueda registrarse si escoge tipo D
bool existeDocente(usuario *listaUsuarios){
	usuario *user = new usuario();
	user = listaUsuarios;
	bool existeDocente = false;
	
	while((user!=NULL)&&(existeDocente==false)){
		if (user->tipoUsuario == "D"){
			existeDocente = true;
		}
		else{
			user = user->proximo;
		}
	}
	return existeDocente;
}

//Funcion que nos permite buscar el usuario correspondiente al momento de iniciar sesion
void buscarUsuarioActual(usuario *listaUsuarios,unsigned long cedula){
	usuarioActual = listaUsuarios;
	
	while (usuarioActual->cedula != cedula){
		if (usuarioActual->cedula != cedula){
			usuarioActual = usuarioActual->proximo;
		}
	}
}

//Funcion que permite guardar en un archivo la lista de los usuarios registrados
void archivarUsuarios(usuario *listaUsuarios){
	ofstream archivoUsuarios;
	usuario *user = new usuario();
	user = listaUsuarios;
	archivoUsuarios.open("listaUsuarios.txt",ios::out);
	if(archivoUsuarios.fail()){
		exit(1);
	}
	else{
    	while(user!= NULL){
    		archivoUsuarios << user->nombres<<endl;
    		archivoUsuarios << user->apellidos<<endl;
    		archivoUsuarios << user->cedula<<endl;
    		archivoUsuarios << user->tipoUsuario<<endl;
    		archivoUsuarios << user->email<<endl;
    		archivoUsuarios << user->clave<<endl;
    		user = user->proximo;
		}
		archivoUsuarios.close();
   }  
}

//Funcion que nos permite leer un archivo de texto con la lista de los usuarios y cargarlos en una lista enlazada, utilizando la funcion de esta misma libreria llamada agregarUsuario
void cargarUsuarios(){
	string nombres, apellidos, email, clave,auxNumero,vacio,tipoUsuario;
	unsigned long numero,cedula;
	ifstream llenado;
	llenado.open("listaUsuarios.txt",ios::in);
	if(llenado.fail()){
		exit(1);
    }
    while(!llenado.eof()){
		getline(llenado,nombres);
   		getline(llenado,apellidos);
    	getline(llenado,auxNumero);
    	istringstream(auxNumero)>>numero;
    	cedula = numero;
    	fflush(stdin);
    	getline(llenado,tipoUsuario); fflush(stdin);
    	getline(llenado,email);
    	getline(llenado,clave);
    	if(!llenado.eof()){
    		agregarUsuario(users,nombres,apellidos,cedula,tipoUsuario,email,clave);
		}
		fflush(stdin);
		if(llenado.eof()){
			break;
		} 	
	}	
    llenado.close();
}
