#include<iostream>

using namespace std;

struct respuestaOpcion{
	int codigo;
	respuestaOpcion *proximo;
};

respuestaOpcion *listaOpciones =NULL;

//Funcion que asigna un nodo dentro de la lista respuestaOpcion si y solo si la respuesta no ha sido ingresada anteriormente (por eso se llama en la siguiente funcion)
//NOTA: cuando se pase de pregunta esta lista se borrara en el main para no generar errores.
void asignarOpcion(respuestaOpcion *&listaOpciones,int opcionRespuesta){
	respuestaOpcion *guardarOpcion = new respuestaOpcion();
	
	guardarOpcion->codigo =opcionRespuesta;
	
	respuestaOpcion *auxiliar1 = listaOpciones;
	respuestaOpcion *auxiliar2;
	
	while(auxiliar1!=NULL){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->proximo;
	}
	
	if (listaOpciones == auxiliar1){
		listaOpciones = guardarOpcion;
	}
	else{
		auxiliar2->proximo = guardarOpcion;
	}
	
	guardarOpcion->proximo= auxiliar1;
}

//Funcion de tipo boolean que dara la confirmacion si el usuario esta volviendo a ingresar una respuesta antes dada en el tipo de pregunta 
//Seleccion Multiple o al momento de ingresar las preguntas creando un examen. Donde se comparara la opcion ingresada por el usuario, con la lista de respuestas creada; si esta es igual
// a alguna en esta lista, retornara true y no se asignara la opcion a la lista
// NOTA: en Verdadero y Falso tambien se invoca para asginar la opcion a la lista, para que no genere errores en un condicional
// ubicado en el main.
bool noRepetida(respuestaOpcion *&listaOpciones, int opcionRespuesta){
	bool validacion=false;
	respuestaOpcion *opcionAverificar = new respuestaOpcion();
	opcionAverificar = listaOpciones;
	
	if(opcionAverificar!=NULL){
		while((opcionAverificar != NULL)&&(validacion!=true)){
			if(opcionAverificar->codigo==opcionRespuesta){
				validacion = true;
			}else{
				opcionAverificar = opcionAverificar->proximo;
			}
		}
		if(validacion==false){
			asignarOpcion(listaOpciones,opcionRespuesta);
		}
	}else{
		validacion = false;
		asignarOpcion(listaOpciones,opcionRespuesta);
	}
	return validacion;
}
