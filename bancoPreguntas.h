#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

struct respuesta{
	int codigo;
	string respuestaPosible; //nos guarda la opcion de respuesta
	string respuestaCorrecta; //nos guarda "S" o "N" para saber si la respuesta es correcta o no
	respuesta *siguiente;
};

struct pregunta{
	int numeroResp;
	int cantRespuestasCorrectas; //guardamos la cantidad de respuestas correctas para el factor de correccion 
	int codigo;
	string enunciado;
	string tipo;
	respuesta *respuestasPosibles; //guardamos la lista de respuestas posibles a la pregunta
	pregunta *siguiente;
};

pregunta *bancoPreguntas = NULL;//Nodo cabeza de la lista del banco de preguntas

respuesta *listaResp = NULL; //Auxiliar para cada vez que creemos una lista de respuestas para cada pregunta, cuando la direccuin de memoria que tenga se le asigne al respectivo campo de la pregunta, vuelve a apuntar a NULL

//Funcion que nos verifica si existe un banco de preguntas
bool existeBancoPreguntas(pregunta *bancoPreguntas){
	if (bancoPreguntas == NULL){
		return false;
	}
	else{
		return true;
	}
}

//Funcion que nos verifica si el codigo de pregunta ingresado por el usuario existe en el banco de preguntas
bool existeCodigoPregunta(pregunta *bancoPreguntas, int codigo){
	pregunta *preguntaActual = new pregunta();
	preguntaActual = bancoPreguntas;
	bool existe=false;
	
	while ((preguntaActual != NULL)&&(existe==false)){
		if (preguntaActual->codigo == codigo){
			existe = true;
		}
		else{
			existe = false;
		}
		preguntaActual = preguntaActual->siguiente;
	}
	
	return existe;
}

//Antes de agregar una pregunta al banco de preguntas, agregamos las respuestas posibles que el docente ingrese al auxiliar listaResp
//Cuando termine de ingresar las respuestas, se crea la pregunta (funcion siguiente) y, ya asignada la lista de respuestas a su campo correspondiente
//listaResp vuelve a apuntar a NULL 
void agregarRespuesta(respuesta *&listaRespuestas, int codigo, string respuestaPosible, string respuestaCorrecta){
	respuesta *nuevaRespuesta = new respuesta();
	
	nuevaRespuesta->codigo = codigo;
	nuevaRespuesta->respuestaPosible = respuestaPosible;
	nuevaRespuesta->respuestaCorrecta = respuestaCorrecta;
	
	respuesta *auxiliar1 = listaRespuestas;
	respuesta *auxiliar2;
	
	while (auxiliar1 != NULL){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	}
	
	if (listaRespuestas == auxiliar1){
		listaRespuestas = nuevaRespuesta;
	}
	else{
		auxiliar2->siguiente = nuevaRespuesta;
	}
	
	nuevaRespuesta->siguiente = auxiliar1;
}

//Teniendo una lista de respuestas ya creada, junto a los datos del nodo de una pregunta se agrega al banco de preguntas
//Se agregan los nodos por orden de codigo
void agregarPregunta(pregunta *&bancoPreguntas, respuesta *listaRespuestas, string enunciado, string tipoPregunta, int codigoPregunta, int nResp, int contRespCorrectas){
	pregunta *nuevaPregunta = new pregunta();
	
	nuevaPregunta->numeroResp =nResp;
	nuevaPregunta->codigo = codigoPregunta;
	nuevaPregunta->enunciado = enunciado;
	nuevaPregunta->tipo = tipoPregunta;
	nuevaPregunta->cantRespuestasCorrectas = contRespCorrectas;
	nuevaPregunta->respuestasPosibles = listaRespuestas;
	
	pregunta *auxiliar1 = bancoPreguntas;
	pregunta *auxiliar2;
	
	while((auxiliar1 != NULL)&&(auxiliar1->codigo < codigoPregunta)){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	}
	
	if (bancoPreguntas == auxiliar1){
		bancoPreguntas = nuevaPregunta;
	}
	else{
		auxiliar2->siguiente = nuevaPregunta;
	}
	
	nuevaPregunta->siguiente = auxiliar1;
}

//Funcion que permite modificar los datos de una pregunta (en caso de que no este presente en un examen)
//Busca el codigo de la pregunta que ingreso el usuario, y cuando lo encuentra le asigna los respectivos cambios
void modificarPregunta(pregunta *&bancoPreguntas,int codigoPreguntas, respuesta *listaRespuesta, string enunciadoPregunta, string tipoPregunta, int contRespCorrectas){
	pregunta *preguntaActual = new pregunta();
	preguntaActual = bancoPreguntas;
	while(preguntaActual!=NULL){
		if(preguntaActual->codigo == codigoPreguntas){		
			preguntaActual->enunciado=enunciadoPregunta;
			preguntaActual->tipo = tipoPregunta;
			preguntaActual->cantRespuestasCorrectas = contRespCorrectas;
			preguntaActual->respuestasPosibles=listaRespuesta;
		}
		preguntaActual = preguntaActual->siguiente;	
	}
}

//Busca el codigo de la pregunta a eliminar, recorriendo la lista con dos auxiliares y cuando encuentra el codigo
//hace que la pregunta anterior a la que se va a eliminar, apunte a la siguiente y luego elimina la actual
void eliminarPregunta(pregunta *&bancoPreguntas,int codigoPreguntas){
	pregunta *auxiliar1 = bancoPreguntas;
	pregunta *auxiliar2 = NULL;

	while ((auxiliar1 != NULL) && (codigoPreguntas != auxiliar1->codigo)){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;			
	}

	if (auxiliar2 == NULL){
		bancoPreguntas = bancoPreguntas->siguiente;
		delete auxiliar1;	
	}
	else{
		auxiliar2->siguiente = auxiliar1->siguiente;
		delete auxiliar1;
	}

}

//Muestra el banco de preguntas completo
void mostrarListaPreguntas(pregunta *bancoPreguntas){
	pregunta *preguntaActual = new pregunta();
	respuesta *respuestaActual = new respuesta();
	preguntaActual = bancoPreguntas;
	while(preguntaActual != NULL){
		cout<<"\n----------------------------------------------------"<<endl;
		cout<<"Codigo: "<<preguntaActual->codigo<<endl;
		cout<<"Enunciado: "<<preguntaActual->enunciado<<endl;
		cout<<"Tipo: "<<preguntaActual->tipo<<endl;
		respuestaActual = preguntaActual->respuestasPosibles;
		if(preguntaActual->tipo == "SM"){
			cout<<"Respuestas posibles: "<<endl;
			while(respuestaActual != NULL){
				cout<<"\t\t\t"<<respuestaActual->respuestaPosible<<endl;
				respuestaActual = respuestaActual->siguiente;
			}
		}
		else{
			cout<<"Respuesta: "<<respuestaActual->respuestaPosible<<endl;
		}
		preguntaActual = preguntaActual->siguiente;
	}
}

//Muestra una sola pregunta (para cuando la modifique)
void mostrarPregunta(pregunta *bancoPreguntas,int codigoPreguntas){
	pregunta *preguntaActual = new pregunta();
	preguntaActual = bancoPreguntas;
	respuesta *respuestaModificada = new respuesta();

	
	while(preguntaActual!=NULL){
		if(preguntaActual->codigo == codigoPreguntas){
			cout<<"\n----------------------------------------------------"<<endl;
			cout<<"Codigo: "<<preguntaActual->codigo<<endl;
			cout<<"Enunciado: "<<preguntaActual->enunciado<<endl;
			cout<<"Tipo: "<<preguntaActual->tipo<<endl;
			respuestaModificada = preguntaActual->respuestasPosibles;
			if(preguntaActual->tipo == "SM"){
				cout<<"Respuestas posibles: "<<endl;
				while(respuestaModificada != NULL){
					cout<<"\t\t\t"<<respuestaModificada->respuestaPosible<<endl;
					respuestaModificada = respuestaModificada->siguiente;
				}
			}
			else{
				cout<<"Respuesta: "<<respuestaModificada->respuestaPosible<<endl;
			}
		}
		preguntaActual = preguntaActual->siguiente;
	}
	
}

//Funcion que me archivara las preguntas creadas por el profesor
//Donde se guarda de un orden especifico para poder cargarlas sin errores
// este orden es: numero de respuestas posibles que tiene cada pregunta, el tipo de la pregunta 
// la cantidad de respuestas correctas en cada pregunta, se guardan las respuestas de cada pregunta,
// de ultimo se guardaran el codgio de cada pregunta y su ennuciado correspondiente.
void archivarPreguntas(pregunta *bancoPreguntas){ 
   	pregunta *archiPregunta = new pregunta();
   	respuesta *archiResp = new respuesta();
   	archiPregunta = bancoPreguntas;
   	ofstream archivoUsuarios;
   	archivoUsuarios.open("bancoDePreguntas.txt",ios::out);
   	if(archivoUsuarios.fail()){
		exit(1);	
	}
   	while(archiPregunta!=NULL){
   	    	
   		archiResp = archiPregunta->respuestasPosibles;
   		archivoUsuarios<< archiPregunta->numeroResp<<endl;
   		archivoUsuarios << archiPregunta->tipo<<endl;
   		archivoUsuarios << archiPregunta->cantRespuestasCorrectas<<endl;
   		while(archiResp != NULL){
   			archivoUsuarios << archiResp->codigo<<endl;
   			archivoUsuarios << archiResp->respuestaPosible<<endl;
			archivoUsuarios << archiResp->respuestaCorrecta<<endl;	
   			archiResp = archiResp->siguiente;
		}
   		archivoUsuarios << archiPregunta->codigo<<endl;
		archivoUsuarios << archiPregunta->enunciado<<endl;
		archiPregunta = archiPregunta->siguiente;	
	}
   	archivoUsuarios.close();
}

//Funcion que permitira cargar las preguntas anteriormente creadas por el profesor 
// en esta funcion el metodo de carga se caracteriza por reutilizar funciones para crear 
// la lista del banco de preguntas, estas son: AGREGAR RESPUESTA y AGREGAR PREGUNTA(cada una
// esta explicada en su propio apartado). Con el orden que se guardo nos permitira 
// cargar las preguntas volviendo a crear la lista correspondiente al banco de preguntas
// con dichas funciones.
void cargarPreguntas(){
	string  enunciadoPregunta, tipoPregunta, respuestaPosible, respuestaCorrecta, auxCodigo,auxCont,auxCodigoResp,auxCantPreguntasCorrectas;
	int codigoPregunta,codigoRespuesta,nResp,contRespuestasCorrectas;
	
	ifstream llenado;
	llenado.open("bancoDePreguntas.txt",ios::in);
	if(llenado.fail()){
		exit(1);
    }
    while(!llenado.eof()){
		getline(llenado,auxCont);
		istringstream(auxCont)>>nResp;
		fflush(stdin);
		getline(llenado,tipoPregunta);
		getline(llenado,auxCantPreguntasCorrectas);
		istringstream(auxCantPreguntasCorrectas)>>contRespuestasCorrectas;
		fflush(stdin);
		if(!llenado.eof()){
			if(tipoPregunta =="SM"){
			    for(int i=1;i<=nResp;i++){
			    	fflush(stdin);
			    	getline(llenado,auxCodigoResp);
			    	istringstream(auxCodigoResp)>>codigoRespuesta; fflush(stdin);
			  		getline(llenado,respuestaPosible);
					getline(llenado,respuestaCorrecta);
					agregarRespuesta(listaResp, codigoRespuesta, respuestaPosible, respuestaCorrecta);	
				}		
				
			}
			else{
				getline(llenado,auxCodigoResp);
				istringstream(auxCodigoResp)>>codigoRespuesta; fflush(stdin);
				getline(llenado,respuestaPosible);
				getline(llenado,respuestaCorrecta);
				agregarRespuesta(listaResp, 1, respuestaPosible, respuestaCorrecta);	
			}
		}
		    
		getline(llenado,auxCodigo);
		istringstream(auxCodigo)>>codigoPregunta;
		fflush(stdin);	
		getline(llenado,enunciadoPregunta);
		if(!llenado.eof()){
			agregarPregunta(bancoPreguntas, listaResp, enunciadoPregunta, tipoPregunta, codigoPregunta,nResp,contRespuestasCorrectas);
			listaResp = NULL;
		}
		if(llenado.eof()){
			listaResp = NULL;
			break;
		}
	}
	llenado.close();
}


