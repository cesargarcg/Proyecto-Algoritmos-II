#include <iostream>
#include <cstdlib>
#include "examen.h"

using namespace std;


struct respuestasEx{
	int codigoRespEstudiante;
	respuestasEx *siguiente;
};

//Estuctura que servira para guardar las preguntas de un examen contestado junto a las respuestas que un estudiante dio.
struct preguntasEx{
	int codigoPreg;
	int cantRespEst;
	respuestasEx *respuestasEstudiante;
	preguntasEx *siguiente;
};

struct presentado{
	int codigoExamen;
	unsigned long cedula;             
	string fecha;
	string horaInicio;
	string horaFin;
	float nota;
	float duracion;
	preguntasEx *preguntasExamenPresentado;
	presentado *siguiente;
};

presentado *listaExamenesPresentados = NULL;
preguntasEx *listaPreguntasPresentado = NULL;


bool existeListaExamenesPresentados(presentado *listaExamenesPresentados){
	if(listaExamenesPresentados == NULL){
		return false;
	}
	else{
		return true;
	}
}

//Funcion para saber si algun examen en concreto ya fue respondido.
bool verificarExamenPresentado(presentado *listaExamenesPresentados,int codigoExamen){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	bool existe=false;
	
	while((presentadoActual!=NULL)&&(existe == false)){
		if(presentadoActual->codigoExamen == codigoExamen){
			existe = true;
		}
		presentadoActual = presentadoActual->siguiente;
	}
	
	return existe;
}

//Funcion de tipo boolean que me dira si un estudiante ya presento algun examen (cualquiera)
bool cedulaPresento(presentado *listaExamenesPresentados, unsigned long cedula){
	presentado *presentadoActual = listaExamenesPresentados;
	bool existe = false;
	
	while((presentadoActual!=NULL)&&(existe==false)){
		if(presentadoActual->cedula == cedula){
			existe = true;
		}
		presentadoActual = presentadoActual->siguiente;
	}
	
	return existe;
}

//Funcion que me retornara un valor booleano para confirmar la realizacion de un examen en concreto por un estudiante.
bool cedulaPresentoExamen (presentado *listaExamenesPresentados, unsigned long cedula, int codigo){
	presentado *presentadoActual = listaExamenesPresentados;
	bool existe = false;
	
	while((presentadoActual!=NULL)&&(!existe)){
		if((presentadoActual->cedula == cedula)&&(presentadoActual->codigoExamen == codigo)){
			existe = true;
		}
		presentadoActual = presentadoActual->siguiente;
	}
	return existe;
}

//Cuando inicia un examen, creamos una lista de preguntas que sus nodos solo van a contener el codigo de la pregunta con las respuestas que dio el usuario a esas preguntas
void agregarPreguntasPresentado(preguntasEx *&listaPreguntasPresentado, examen *examenActual){
	pregunta *preguntasExamen = examenActual->preguntasExamen;
	while(preguntasExamen!=NULL){
		preguntasEx *nuevaPregunta = new preguntasEx();
		nuevaPregunta->codigoPreg = preguntasExamen->codigo;
		nuevaPregunta->siguiente = NULL;
		
		preguntasEx *auxiliar1 = listaPreguntasPresentado;
		preguntasEx *auxiliar2 = NULL;
		while(auxiliar1!=NULL){
			auxiliar2 = auxiliar1;
			auxiliar1 = auxiliar1->siguiente;
		}
		if(listaPreguntasPresentado == auxiliar1){
			listaPreguntasPresentado = nuevaPregunta;
		}
		else{
			auxiliar2->siguiente = nuevaPregunta;
		}
		
		preguntasExamen = preguntasExamen->siguiente;
	}
}

//Cuando el usuario selecciona una respuesta en una pregunta del examen, la guardamos en el nodo de una lista de respuestas de una pregunta para 
//mostrarselas en la opcion 5 del menu y asi poder comparar sus respuestas con las correctas de las preguntas
void agregarRespuestasPreguntas(preguntasEx *&listaPreguntasPresentado,int codigoRespEstudiante,int codigoPregunta){
	respuestasEx *nuevaRespuesta = new respuestasEx();
	preguntasEx *auxiliar = listaPreguntasPresentado;
	nuevaRespuesta->codigoRespEstudiante = codigoRespEstudiante;
	nuevaRespuesta->siguiente = NULL;
	
	while(auxiliar!=NULL){
		if(auxiliar->codigoPreg == codigoPregunta){
			if(auxiliar->respuestasEstudiante == NULL){
				auxiliar->respuestasEstudiante = nuevaRespuesta;
			}
			else{
				respuestasEx *auxiliar2 = auxiliar->respuestasEstudiante;
				while(auxiliar2->siguiente != NULL){
					auxiliar2 = auxiliar2->siguiente;
				}
				auxiliar2->siguiente = nuevaRespuesta;
			}
		}
		auxiliar = auxiliar->siguiente;
	}
	
}

//Funcion que  permitira guardar la cantidad de respuestas dadas en un examen en una pregunta en concreto por un estudiante, esto lo agregamos para al cargar
// la lista desde un archivo saber cuantas respuestas hay por cada pregunta y poder establecer la condicion del ciclo de carga. 
void agregarCantidadRespuestasEstudiante(preguntasEx *&listaPreguntasPresentado,int codigoPregunta,int cantRespEst){
	preguntasEx *preguntaActual = new preguntasEx();
	preguntaActual = listaPreguntasPresentado;
	while(preguntaActual != NULL){
		if(preguntaActual->codigoPreg == codigoPregunta){
			preguntaActual->cantRespEst = cantRespEst;
		}
		preguntaActual = preguntaActual->siguiente;
	}
}

//Funcion que va a agregar un nodo de examen presentado a la lista de los examenes presentados, para cada estudiante(Se agrega por orden de cedula). 
void agregarPresentado(presentado *&listaExamenesPresentados,int codigo, unsigned long cedula,float nota, float duracion,string horaInicio,string horaFinal,string fecha,preguntasEx *&listaPreguntasPresentado){
	presentado *nuevoPresento = new presentado();
	
	nuevoPresento->cedula = cedula;
	nuevoPresento->codigoExamen = codigo;
	nuevoPresento->fecha = fecha;
	nuevoPresento->horaInicio = horaInicio;
	nuevoPresento->horaFin = horaFinal;
	nuevoPresento->nota = nota;
	nuevoPresento->duracion = duracion;
	nuevoPresento->preguntasExamenPresentado = listaPreguntasPresentado;
	listaPreguntasPresentado = NULL;
	
	presentado *auxiliar1 = listaExamenesPresentados;
	presentado *auxiliar2 = NULL;
	
	while((auxiliar1!=NULL)&&(auxiliar1->cedula < cedula)){
		auxiliar2 = auxiliar1;
		auxiliar1 = auxiliar1->siguiente;
	}
	
	if (listaExamenesPresentados == auxiliar1){
		listaExamenesPresentados = nuevoPresento;
	}
	else{
		auxiliar2->siguiente = nuevoPresento;
	}
	
	nuevoPresento->siguiente = auxiliar1;	
}

//Funcion que mostrara la lista de los examenes presentados por todos los estudiantes junto a su nota.
void mostrarListaExamenesPresentados(presentado *listaExamenesPresentados){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	while(presentadoActual!=NULL){
		cout<<"********************************"<<endl;
		cout<<"Cedula del alumno que presento: "<<presentadoActual->cedula<<endl;
		cout<<"Codigo de examen: "<<presentadoActual->codigoExamen<<endl;
		buscarExamenActual(listaExamenes,presentadoActual->codigoExamen);
		cout<<"Titulo del examen: "<<examenActual->titulo<<endl;
		examenActual = NULL;
		cout<<"Fecha de realizacion: "<<presentadoActual->fecha<<endl;
		cout<<"Hora de inicio: "<<presentadoActual->horaInicio<<endl;
		cout<<"Hora al finalizar: "<<presentadoActual->horaFin<<endl;
		cout<<"Duracion: "<<presentadoActual->duracion<<" minutos"<<endl;
		cout<<"Nota obtenida: "<<presentadoActual->nota<<endl;
		cout<<"********************************\n"<<endl;
		presentadoActual = presentadoActual->siguiente;
	}
}

//Funcion que mostrara la lista de examenes que presento un estudiante(sin nota).
void mostrarCodigosExamenesPresentadosEstudiante(presentado *listaExamenesPresentados, unsigned long cedula){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	while(presentadoActual!=NULL){
		if (presentadoActual->cedula == cedula){
			cout<<"*********************************"<<endl;
			cout<<"Codigo de examen: "<<presentadoActual->codigoExamen<<endl;
			buscarExamenActual(listaExamenes,presentadoActual->codigoExamen);
			cout<<"Titulo del examen: "<<examenActual->titulo<<endl;
			examenActual = NULL;
			cout<<"Fecha de realizacion: "<<presentadoActual->fecha<<endl;
			cout<<"Hora de inicio: "<<presentadoActual->horaInicio<<endl;
			cout<<"Hora al finalizar: "<<presentadoActual->horaFin<<endl;
			cout<<"Duracion: "<<presentadoActual->duracion<<" minutos"<<endl;
			cout<<"*********************************\n"<<endl;
		}	
		presentadoActual = presentadoActual->siguiente;
	}	
}

//Funcion para el profesor que va a mostrar todos los examenes presentados por un estudiante.
void mostrarExamenesEstudiante(presentado *listaExamenesPresentados, unsigned long cedula){ 
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	while(presentadoActual!=NULL){
		if (presentadoActual->cedula == cedula){
			cout<<"*********************************"<<endl;
			cout<<"Codigo de examen: "<<presentadoActual->codigoExamen<<endl;
			buscarExamenActual(listaExamenes,presentadoActual->codigoExamen);
			cout<<"Titulo del examen: "<<examenActual->titulo<<endl;
			examenActual = NULL;
			cout<<"Fecha de realizacion: "<<presentadoActual->fecha<<endl;
			cout<<"Hora de inicio: "<<presentadoActual->horaInicio<<endl;
			cout<<"Hora al finalizar: "<<presentadoActual->horaFin<<endl;
			cout<<"Duracion: "<<presentadoActual->duracion<<" minutos"<<endl;
			cout<<"Nota obtenida: "<<presentadoActual->nota<<endl;
			cout<<"*********************************\n"<<endl;
		}
		presentadoActual = presentadoActual->siguiente;
	}
}

//Funcion que mostrara las notas de todos los examenes presentado por un estudiante.
void mostrarNotasEstudiante(presentado *listaExamenesPresentados, unsigned long cedula){ 
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	while(presentadoActual!=NULL){
		if (presentadoActual->cedula == cedula){
			cout<<"*********************************"<<endl;
			cout<<"Codigo de examen: "<<presentadoActual->codigoExamen<<endl;
			buscarExamenActual(listaExamenes,presentadoActual->codigoExamen);
			cout<<"Titulo del examen: "<<examenActual->titulo<<endl;
			examenActual = NULL;
			cout<<"Nota obtenida: "<<presentadoActual->nota<<endl;
			cout<<"*********************************\n"<<endl;
		}
		presentadoActual = presentadoActual->siguiente;
	}
}

//Funcion para mostrar la nota de un estudiante en un examen.
void mostrarNotaEstudiante(presentado *listaExamenesPresentados,unsigned long cedula,int codigoExamen){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	while(presentadoActual!=NULL){
		if((presentadoActual->cedula == cedula)&&(presentadoActual->codigoExamen == codigoExamen)){
			cout<<"*********************************"<<endl;
			cout<<"Codigo de examen: "<<presentadoActual->codigoExamen<<endl;
			buscarExamenActual(listaExamenes,presentadoActual->codigoExamen);
			cout<<"Titulo del examen: "<<examenActual->titulo<<endl;
			examenActual = NULL;
			cout<<"Nota obtenida: "<<presentadoActual->nota<<endl;
			cout<<"*********************************\n"<<endl;
		}
		presentadoActual = presentadoActual->siguiente;
	}
}

//Recorremos la lista de examenes presentados y cuando encuentre el que tiene la cedula del estudiante y el codigo del examen que ingreso hace lo siguiente
//Muestra el numero de pregunta junto a su enunciado y las respuestas posibles (por lo que tenemos que buscar la pregunta en el banco de preguntas)
//Ademas, se le muestra si la respuesta posible es correcta o no, con un condicional ya que guardamos cada respuesta posible con un campo llamado respuesta correcta
//que cuando ese campo tiene una S significa que si es correcta, en cambio, si tiene una N no lo es
//Luego de haber mostrado lo anterior, creamos un auxiliar del tipo respuestasEx que nos va a recorrer la lista de respuestas que el usuario ingreso
//De esta manera, el estudiante puede comparar el resultado de su examen
void mostrarPreguntasYRespuestasDelEstudiante(presentado *listaExamenesPresentados,unsigned long cedula,int codigoExamen){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	
	int contador = 1;
	
	while(presentadoActual!=NULL){
		if((presentadoActual->cedula == cedula)&&(presentadoActual->codigoExamen == codigoExamen)){
			preguntasEx *listaPreguntas = presentadoActual->preguntasExamenPresentado;
			while(listaPreguntas!=NULL){
				pregunta *auxListaPreguntas = bancoPreguntas, *auxListaPreguntas2 = NULL;
				
				//buscamos la pregunta con el codigo en el banco de preguntas
				while(auxListaPreguntas!=NULL){
					if(auxListaPreguntas->codigo == listaPreguntas->codigoPreg){
						auxListaPreguntas2 = auxListaPreguntas;
					}
					auxListaPreguntas = auxListaPreguntas->siguiente;
				}
				
				listaResp = auxListaPreguntas2->respuestasPosibles;
				
				cout<<"***********************************************"<<endl;
				cout<<"PREGUNTA "<<contador<<endl<<endl;
				cout<<"Enunciado: "<<auxListaPreguntas2->enunciado<<endl;
				cout<<"Respuestas posibles: \n"<<endl;
				while(listaResp!=NULL){
					cout<<listaResp->codigo<<". "<<listaResp->respuestaPosible<<". Respuesta correcta: ";
					if(listaResp->respuestaCorrecta == "S"){
						cout<<"Si"<<endl;
					}
					else{
						cout<<"No"<<endl;
					}
					listaResp = listaResp->siguiente;
				}
				if(auxListaPreguntas2->tipo == "VF"){
					if(auxListaPreguntas2->respuestasPosibles->respuestaPosible == "V"){
						cout<<"2. F. Respuesta correcta: No"<<endl;
					}
					else{
						cout<<"2. V. Respuesta correcta: No"<<endl;
					}
				}
				
				respuestasEx *listaRespuestas = listaPreguntas->respuestasEstudiante;
				if(listaRespuestas == NULL){
					cout<<"\nUsted no respondio esta pregunta.";
				}
				else{
					cout<<"\nUsted respondio las opciones: ";
					while(listaRespuestas!=NULL){
						cout<<listaRespuestas->codigoRespEstudiante;
						if(listaRespuestas->siguiente != NULL){
							cout<<", ";
						}
						listaRespuestas = listaRespuestas->siguiente;
					}
				}
				cout<<"\n***********************************************\n"<<endl;
				listaPreguntas = listaPreguntas->siguiente;
				contador++;
			}
		}
		presentadoActual = presentadoActual->siguiente;
	}
}

//Funcion que le servira al estudiante para saber el promedio de TODAS sus notas de los examenes presentados por el mismo.
// En esta funcion lo que se va  realizar es la busqueda por la cedula del estudinate sus examenes presentados en la lista de los
// examenes presentados, al ubicar sus examenes se contara cuantos examenes contesto y se va a sumar cada una de sus notas.
// Al final se hara una division de la suma de sus notas entre la cantidad de examenes respondidos por el estudiante.
float promedioDeNotasEstudiante(presentado *listaExamenesPresentados, unsigned long cedula){ 
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	int cantidadPresentados=0;
	float suma = 0,promedio;
	
	while(presentadoActual!=NULL){
		if(presentadoActual->cedula == cedula){
			cantidadPresentados++;
			suma+= presentadoActual->nota;
		}
		presentadoActual = presentadoActual->siguiente;
	}
	
	promedio = suma/cantidadPresentados;
	
	return promedio;
}


//Funcion para el profesor que le permitira ver el promedio de las notas de un examen presentado por los estudiantes 
// Donde se pasara el codigo del examen a consultar el promedio, se buscara en la lista de los examenes ya presentados
//	el codigo ingresado, cuando se encuentre se recorrera la lista de los examenes presentados donde se va ir sumando un contador
//  de los estudiantes que ya lo presentaron y al mismo timepo una suma que me va a guardar la suma de todas las notas 
// de cada estudiante; al final se divide la suma de las notas entre la cantidad de estudiantes que presentaron el examen.
// El codigo se va a ir ejecutando en el main con cada examen que este en la lista de examenes.
float promedioDeNotasEstudiantes(presentado *listaExamenesPresentados,int codigoExamen){
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	int cantidadPresentados=0;
	float suma = 0,promedio;
	
	while(presentadoActual != NULL){
		if(presentadoActual->codigoExamen == codigoExamen){
			cantidadPresentados++;
			suma+= presentadoActual->nota;
		}
		presentadoActual = presentadoActual->siguiente;
	}	
	promedio = suma/cantidadPresentados;
	return promedio;
}

//Esta funcion es para que el usuario que este en el rol de profesor pueda ver el promedio de la duracion de cada uno de los examenes 
// a la duracion de que hayan sido respondidos, donde se pasara  en el main el codigo del examen que se desea solicitar el promedio
// con este codigo se va a buscar en la lista de los examenes presentados y cuando se encuentre el examen solicitado se busca dentro de la lista la duracion para cada estudiante
// donde se sumara un contador de cada estudiante que haya presentado y luego este sera el divisor ante la suma de las duraciones por cada estudiante.
float promedioDeDuracionExamenes(presentado *listaExamenesPresentados,int codigoExamen){ 
	presentado *presentadoActual = new presentado();
	presentadoActual = listaExamenesPresentados;
	int cantidadPresentados=0;
	float suma = 0,promedio;
	
	while(presentadoActual != NULL){
		if(presentadoActual->codigoExamen == codigoExamen){
			cantidadPresentados++;
			suma+= presentadoActual->duracion;
		}
		presentadoActual = presentadoActual->siguiente;
	}
	
	promedio = suma/cantidadPresentados;
	
	return promedio;
}

//Funcion que permitira guardar los datos de los examenes que ya han sido presentados en un archivo
//Donde se guardaran datos de los estudaintes presentados que seran su cedula y el codigo del examen que estos presentaron
//Adicional se guardaran datos como la fecha que este los presento, la hora de inicio y la hora de culminacion del examen
//la duracion de la respuesta del alumno, ademas se guardaran los codigos de las preguntas del examen y las respuestas correspondientes a cada una dada por el estudiante.
void guardarExamenesPresentados(presentado *listaExamenesPresentados){
   	presentado *archiPresentados = new presentado();
   	preguntasEx *archiPreguntas = new preguntasEx();
   	respuestasEx *archiResp = new respuestasEx();
   	
   	archiPresentados = listaExamenesPresentados;
   	
   	ofstream archivoExamenesPresentados;
   	archivoExamenesPresentados.open("examenesPresentados.txt",ios::out);
   	if(archivoExamenesPresentados.fail()){
		exit(1);	
	}
   	while(archiPresentados!=NULL){
   		archivoExamenesPresentados << archiPresentados->cedula<<endl;
		archivoExamenesPresentados << archiPresentados->codigoExamen<<endl; 
		archivoExamenesPresentados << archiPresentados->duracion<<endl; 
		archivoExamenesPresentados << archiPresentados->fecha<<endl;
		archivoExamenesPresentados << archiPresentados->horaFin<<endl; 
		archivoExamenesPresentados << archiPresentados->horaInicio<<endl;
		archivoExamenesPresentados << archiPresentados->nota<<endl;
		archiPreguntas = archiPresentados->preguntasExamenPresentado;
		while(archiPreguntas!=NULL){
			archivoExamenesPresentados << archiPreguntas->codigoPreg<<endl;
			archivoExamenesPresentados << archiPreguntas->cantRespEst<<endl;
			archiResp = archiPreguntas->respuestasEstudiante;
			while(archiResp!=NULL){
				archivoExamenesPresentados << archiResp->codigoRespEstudiante<<endl;
				archiResp = archiResp->siguiente;
			}
			archiPreguntas = archiPreguntas->siguiente;
		}	   	
		archiPresentados = archiPresentados->siguiente;	
	}
   	archivoExamenesPresentados.close();
	
}



//Funcion que me cargara el archivo de los respondidos por los estudiantes
// en esta funcion se reutilzaran las funciones para crear las listas correspondientes a la creacion de las mismas para volverlas a cargar
// estas funciones son AGREGAR LAS PREGUNTAS DE LOS PRESENTADOS, AGREGAR RESPUESTAS A PREGUNTAS Y AGREGAR A LISTA DE EXAMEN PRESENTADO
// cada una de estas funciones son explicadas en su propio apartado.
void cargarExamenesPresentados(){
	unsigned long int cedula;
	int codigoExamen,codigoPregunta,codigoRespEstudiante,cantResp;
	float duracion,nota;
	string auxCedula,auxCodExam,auxDuracion,fecha,horaInicio,horaFinal,auxNota,auxCodPreg,auxCodRespEst,auxCantResp;
	preguntasEx *aux = new preguntasEx();
	
	ifstream llenado;
	llenado.open("examenesPresentados.txt",ios::in);
	if(llenado.fail()){
		exit(1);
    }
	while(!llenado.eof()){	
		listaPreguntasPresentado = NULL;
		getline(llenado,auxCedula);
		istringstream(auxCedula)>>cedula; fflush(stdin);
		getline(llenado,auxCodExam);
		istringstream(auxCodExam)>>codigoExamen; fflush(stdin);
		getline(llenado,auxDuracion);
		istringstream(auxDuracion)>>duracion; fflush(stdin);
		getline(llenado,fecha);
		getline(llenado,horaFinal);
		getline(llenado,horaInicio);
		getline(llenado,auxNota);
		istringstream(auxNota)>>nota; fflush(stdin);
		examenActual=NULL;
		buscarExamenActual(listaExamenes,codigoExamen);
		agregarPreguntasPresentado(listaPreguntasPresentado,examenActual);
		aux = listaPreguntasPresentado;
		while(aux!=NULL){
			getline(llenado,auxCodPreg);
			istringstream(auxCodPreg)>>codigoPregunta; fflush(stdin);
			getline(llenado,auxCantResp);
			istringstream(auxCantResp)>>cantResp; fflush(stdin);
			if(cantResp!=0){
				for(int i=1;i<=cantResp;i++){
					getline(llenado,auxCodRespEst);
					istringstream(auxCodRespEst)>>codigoRespEstudiante; fflush(stdin);
					if(!llenado.eof()){
						agregarRespuestasPreguntas(listaPreguntasPresentado,codigoRespEstudiante,codigoPregunta);
					}
				}
			}
			else{
				aux->respuestasEstudiante = NULL;
			}
			if(!llenado.eof()){
				agregarCantidadRespuestasEstudiante(listaPreguntasPresentado,codigoPregunta,cantResp);
			}

			aux = aux->siguiente;
		}
		if(!llenado.eof()){
			agregarPresentado(listaExamenesPresentados,codigoExamen,cedula,nota,duracion,horaInicio,horaFinal,fecha,listaPreguntasPresentado);
			listaPreguntasPresentado = NULL;	
		}
		if(llenado.eof()){
			listaPreguntasPresentado = NULL;
			break;
		}
	}
	llenado.close();			
}
