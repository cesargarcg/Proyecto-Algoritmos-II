#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <ctime>
#include "bancoPreguntas.h"

using namespace std;

struct examen{
	int codigo;
	string titulo;
	int cantidadPreguntas;
	float puntPorPregunta;
	pregunta *preguntasExamen;
	examen *siguiente;
};

examen *listaExamenes = NULL; //nodo cabeza de la lista de examenes

pregunta *listaPreguntas = NULL; //auxiliar que nos permitira asignar una lista de preguntas al examen correspondiente, o para recorrer la lista de preguntas de un examen

examen *examenActual = NULL; //apuntador al examen que se necesite o el que el estudiante va a presentar

//Funcion que nos verifica si hay una pregunta especifica incluida en un examen, para que no se pueda borrar o modificar
bool existePreguntaExamen (examen *listaExamenes,int codigoPregunta){
	examen *nuevoExamen = new examen();
	nuevoExamen = listaExamenes;
	pregunta *buscaPregunta = new pregunta();
	bool existe = false;
	
	while ((nuevoExamen != NULL)&&(!existe)){
		buscaPregunta = nuevoExamen->preguntasExamen;
		while((buscaPregunta !=NULL)&&(!existe)){
			if(buscaPregunta->codigo == codigoPregunta){
				existe = true;
			}
			buscaPregunta = buscaPregunta->siguiente;
		}
		nuevoExamen = nuevoExamen->siguiente;
	}

	return existe;
}

//Funcion que nos verifica si existe una lista de examenes
bool existeListaExamenes(examen *listaExamenes){
	if (listaExamenes==NULL){
		return false;
	}
	else{
		return true;
	}
}

//Funcion que verifica que el codigo del examen ingresado por el usuario exista en la lista de examenes
bool existeCodigoExamen(examen *listaExamenes, int codigoExamen){
	examen *examenActual = new examen();
	examenActual = listaExamenes;
	bool existe=false;
	
	while ((examenActual != NULL)&&(!existe)){
		if (examenActual->codigo == codigoExamen){
			existe = true;
		}
		else{
			existe = false;
		}
		examenActual = examenActual->siguiente;
	}
	
	return existe;
}

//Funcion que nos busca el examen actual que se necesite
void buscarExamenActual(examen *listaExamenes, int codigoExamen){
	examenActual = listaExamenes;
	
	while(examenActual->codigo != codigoExamen){
		if(examenActual->codigo != codigoExamen){
			examenActual = examenActual->siguiente;
		}
	}
}

//Agregamos una copia de una pregunta seleccionada del banco de preguntas a una lista de preguntas distina que sera asignada al examen que se esta creando
//Buscamos el codigo de la pregunta ingresado por el usuario en el banco de preguntas, y copiamos todos los datos de ese nodo en una nuevaPregunta que se agregara
//a la lista de preguntas del examen
void agregarPregExamen(pregunta *&listaPreguntas, pregunta *bancoPreguntas,int codigoPregunta){
	pregunta *nuevaPregunta = new pregunta();
	pregunta *auxiliar = new pregunta();
	auxiliar = bancoPreguntas;
	int codigoResp;
	string respuestaPosible,respuestaCorrecta;
	
	while((auxiliar!=NULL)&&(auxiliar->codigo != codigoPregunta)){
		if(auxiliar->codigo != codigoPregunta){
			auxiliar = auxiliar->siguiente;
		}
	}
	
	nuevaPregunta->codigo = auxiliar->codigo;
	nuevaPregunta->enunciado = auxiliar->enunciado;
	nuevaPregunta->numeroResp = auxiliar->numeroResp;
	nuevaPregunta->tipo = auxiliar->tipo;
	nuevaPregunta->cantRespuestasCorrectas = auxiliar->cantRespuestasCorrectas;
	nuevaPregunta->respuestasPosibles = auxiliar->respuestasPosibles;
	
	pregunta *auxiliar1 = listaPreguntas;
	pregunta *auxiliar2;
	
	while (auxiliar1!=NULL){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	} 
	
	if (listaPreguntas==auxiliar1){
		listaPreguntas = nuevaPregunta;
	}
	else{
		auxiliar2->siguiente = nuevaPregunta;
	}
	
	nuevaPregunta->siguiente = auxiliar1;
}

//Ya creada una lista de preguntas, se agrega el examen con sus preguntas correspondientes a la lista de examenes
//Para sacar el puntaje por pregunta (ya que tiene que ser el mismo), lo que hacemos es dividir 20 (nota maxima) entre el numero de preguntas que el examen contenga
void agregarExamen(examen *&listaExamenes,int codigoExamen,string titulo,int cantidadPreguntas,pregunta *listaPreg){
	examen *nuevoExamen = new examen();
	
	nuevoExamen->cantidadPreguntas = cantidadPreguntas;
	nuevoExamen->codigo = codigoExamen;
	nuevoExamen->puntPorPregunta = 20.0/cantidadPreguntas;
	nuevoExamen->titulo = titulo;
	nuevoExamen->preguntasExamen = listaPreg;
	
	examen *auxiliar1 = listaExamenes;
	examen *auxiliar2;
	
	while(auxiliar1!=NULL){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	}
	
	if (listaExamenes == auxiliar1){
		listaExamenes = nuevoExamen;
	}
	else{
		auxiliar2->siguiente = nuevoExamen;
	}
	
	nuevoExamen->siguiente = auxiliar1;
}

//Buscamos el codigo del examen que el usuario ingreso para eliminar
//cuando lo encuentra, hace que el examen anterior apunte al siguiente del actual y queda eliminado de la lista de examenes
void eliminarExamen(examen *&listaExamenes,int codigoExamen){
	examen *auxiliar1 = listaExamenes;
	examen *auxiliar2 = NULL;
	
	while((auxiliar1 != NULL)&&(auxiliar1->codigo!=codigoExamen)){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	}
	
	if (auxiliar2 == NULL){
		listaExamenes = listaExamenes->siguiente;
		auxiliar1 = NULL;	
	}
	else{
		auxiliar2->siguiente = auxiliar1->siguiente;
		auxiliar1 = NULL;
	}
	
}

//Buscamos el codigo del examen que va a modificar
//Cuando lo encuentra, se le cambian los datos por los nuevos que ingreso el usuario, incluyendo una nueva lista de preguntas
void modificarExamen(examen *&listaExamenes,int codigoExamen,string tituloExamen,int cantidadPreguntas,pregunta *listaPreg){
	examen *examenModificado = new examen();
	examenModificado = listaExamenes;
	while((examenModificado->codigo != codigoExamen) && (examenModificado->siguiente != NULL)){
		examenModificado = examenModificado->siguiente;
	}
	examenModificado->titulo = tituloExamen;
	examenModificado->cantidadPreguntas = cantidadPreguntas;
	examenModificado->puntPorPregunta = 20/cantidadPreguntas;
	examenModificado->preguntasExamen = listaPreg;	
}

//Mostramos la lista completa de examenes al profesor
void mostrarListaExamenes(examen *listaExamenes){
	examen *examenActual = new examen();
	pregunta *preguntaActual = new pregunta();
	respuesta *respuestaActual = new respuesta();
	examenActual = listaExamenes;
	
	while(examenActual!=NULL){
		cout<<"\n**********************************"<<endl;
		cout<<"Codigo de examen: "<<examenActual->codigo<<endl;
		cout<<"Titulo: "<<examenActual->titulo<<endl;
		cout<<"Cantidad de preguntas: "<<examenActual->cantidadPreguntas<<endl;
		cout<<"Puntaje por pregunta: "<<examenActual->puntPorPregunta<<endl;
		cout<<"PREGUNTAS DEL EXAMEN: "<<endl;
		cout<<"------------------------------------"<<endl;
		preguntaActual = examenActual->preguntasExamen;
		while(preguntaActual != NULL){
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
			cout<<"------------------------------------"<<endl;		
			preguntaActual = preguntaActual->siguiente;
		}
		cout<<"**********************************"<<endl;
		examenActual = examenActual->siguiente;
	}
}

//Mostramos los datos de un examen creado (cuando se agrega a la lista de examenes o cuando se modifica)
void mostrarExamenCreado(examen *listaExamenes,int codigoExamen){
	examen *examenActual = new examen();
	pregunta *preguntaActual = new pregunta();
	respuesta *respuestaActual = new respuesta();
	examenActual = listaExamenes;
	
	while((examenActual!=NULL)&&(examenActual->codigo != codigoExamen)){
		examenActual = examenActual->siguiente;
	}
	
	cout<<"**********************************"<<endl;
	cout<<"Codigo de examen: "<<examenActual->codigo<<endl;
	cout<<"Titulo: "<<examenActual->titulo<<endl;
	cout<<"Cantidad de preguntas: "<<examenActual->cantidadPreguntas<<endl;
	cout<<"Puntaje por pregunta: "<<examenActual->puntPorPregunta<<endl;
	cout<<"PREGUNTAS DEL EXAMEN: "<<endl;
	preguntaActual = examenActual->preguntasExamen;
	while(preguntaActual != NULL){
		cout<<"------------------------------------"<<endl;
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
		cout<<"------------------------------------"<<endl;		
		preguntaActual = preguntaActual->siguiente;
	}
	cout<<"**********************************"<<endl;
}

//Mostramos una pequeña informacion sobre los examenes (normalmente cuando se le va a mostrar al estudiante y cuando el docente desea ver la informacion de alguno)
void mostrarExamenesSinDatos(examen *listaExamenes){
	examen *examenActual = new examen();
	examenActual = listaExamenes;
	
	while(examenActual!=NULL){
		cout<<"**************************"<<endl;
		cout<<"Codigo de examen: "<<examenActual->codigo<<endl;
		cout<<"Titulo: "<<examenActual->titulo<<endl;
		cout<<"Cantidad de preguntas: "<<examenActual->cantidadPreguntas<<endl;
		cout<<"**************************\n"<<endl;
		
		examenActual = examenActual->siguiente;
	}
}

//Funcion que me archivara los examenes creados por parte del profesor
// el metodo de guardado tiene un orden particular que nos ayudara a cargar despues 
// el orden para guardar los distintos examenes es, el codigo de cada uno de los examenes,
// la cantidad de preguntas que posee cada examen, el titulo del examen, y el puntaje por cada pregunta
// se guardaran las preguntas del examen junto a su codigo de ultimo.
void archivarExamen(examen *listaExamen){
	examen *archiExamen = new examen();
   	pregunta *archiPreg = new pregunta();
   	respuesta *archiResp = new respuesta();
   	archiExamen = listaExamen;
   	ofstream archivoExamenes;
   	archivoExamenes.open("listaExamenes.txt",ios::out);
   	if(archivoExamenes.fail()){
		exit(1);	
	}
   	while(archiExamen!=NULL){
   	    	archivoExamenes << archiExamen->codigo<<endl;
   	    	archivoExamenes << archiExamen->cantidadPreguntas<<endl;
   	    	archivoExamenes <<archiExamen->titulo<<endl;
   	    	archivoExamenes << archiExamen->puntPorPregunta<<endl;
   	    	archiPreg = archiExamen->preguntasExamen;
   	    	while(archiPreg!=NULL){	
   				archiResp = archiPreg->respuestasPosibles;
   				archivoExamenes << archiPreg->codigo<<endl;
				archiPreg = archiPreg->siguiente;	
			}
			archiExamen =archiExamen->siguiente;
	}
   	archivoExamenes.close();
}


//Funcion que me permitira cargar los examenes que ya han sido creados
// en esta funcion se cargara de una manera que se reutilizaran las funciones 
// de la creacion de los examenes esta son: AGREGAR PREGUNTA EXAMEN y AGREGAR EXAMEN(cada una explicada en su propio apartado).
void cargarExamen(){
	string auxCodExam,auxCantPreg,tituloExam,auxPuntos,auxCodigoPreg;
	int codigoExam,cantidadPreguntas,codigoPreg;
	float puntPorPreg;
	
	ifstream llenado;
	llenado.open("listaExamenes.txt",ios::in);
	if(llenado.fail()){
		exit(1);
    }
	while(!llenado.eof()){
		getline(llenado,auxCodExam);
		istringstream(auxCodExam)>>codigoExam; fflush(stdin);
		getline(llenado,auxCantPreg);
		istringstream(auxCantPreg)>>cantidadPreguntas; fflush(stdin);
		getline(llenado,tituloExam);
		getline(llenado,auxPuntos);
		istringstream(auxPuntos)>>puntPorPreg; fflush(stdin);	
		for(int contador=1;contador<=cantidadPreguntas;contador++){ // Ciclo que depende de la cantidad de preguntas del examen para poderlas agregar.
			getline(llenado,auxCodigoPreg);							
			istringstream(auxCodigoPreg)>>codigoPreg; fflush(stdin);
			agregarPregExamen(listaPreguntas,bancoPreguntas,codigoPreg);
		}
		if(!llenado.eof()){
			agregarExamen(listaExamenes,codigoExam,tituloExam,cantidadPreguntas,listaPreguntas);
			listaPreguntas = NULL;		//Al ya crear el examen se reinicia la lista de preguntas del examen para reutilizarla a la hora de cargar otro examen.
		}
		if(llenado.eof()){
			listaPreguntas = NULL;		//Se reinicia una ultima vez la lista de preguntas, para que el profesor no tenga ningun inconveniente al crear otro examen.
			break;
		}
	}
	llenado.close();										
}


