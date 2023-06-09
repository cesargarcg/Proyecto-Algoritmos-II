#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "usuario.h"
#include "examenPresentado.h"
#include "validacionOpciones.h"

using namespace std;

//FUNCIONES EXPLICADAS AL FINAL
void clrscr();
bool verificacionEmail(string);
bool verificacionContra(string);
bool verificarTipo(string);
bool verVerdaderoFalso(string);
bool verResp(string);
bool preguntaCorrecta(string);
string mayuscula(string);
float tiempoActual();
string enteroString(int);

int main(){
	int opMenu1, opMenuDocente, opMenuEstudiante, codigoPregunta, cantidadRespuestas,contPreguntas,contRespCorrectas=0,codigoExam,cantidadPreguntas,auxCantPreguntas,auxHora,auxMinutos,auxDia,auxMes,auxAnio,auxCantRespEst,auxVF;
	float tInicial,tFinal,duracion,notaExamen = 0,notaActual = 0;
	string nombres, apellidos, tipoUsuario, email, clave, enunciadoPregunta, tipoPregunta, respuestaPosible, respuestaCorrecta,tituloExamen,hora,minutos,horaInicio,horaFinal,dia,mes,anio,fechaExamen;
	unsigned long cedula;
	bool tipoCorrecto,emailCorrecto,contraCorrecta,validarOpcion,existeRespCorrecta;
	ifstream llenadoUsuarios, llenadoPreguntas,llenadoExamenes,llenadoExamenesPresentados;
	respuesta *auxListaResp; //lo usamos para eliminar una lista de respuestas
	time_t now;
	tm *tiempo;
	
	llenadoUsuarios.open("listaUsuarios.txt",ios::in);
	if(!llenadoUsuarios.fail()){                                                     // cargamos todos los datos de los archivos
		cargarUsuarios();
	}
	llenadoUsuarios.close();
	llenadoPreguntas.open("bancoDePreguntas.txt",ios::in);
	if(!llenadoPreguntas.fail()){
		cargarPreguntas();
		llenadoExamenes.open("listaExamenes.txt",ios::in);
		if(!llenadoExamenes.fail()){
			cargarExamen(); fflush(stdin);
			llenadoExamenesPresentados.open("examenesPresentados.txt",ios::in);
			if(!llenadoExamenesPresentados.fail()){
				cargarExamenesPresentados();fflush(stdin);
			}
			llenadoExamenesPresentados.close();
		}
		llenadoExamenes.close();
	}
	llenadoPreguntas.close();	
	
	do{
		clrscr();
		cout<<"\t\tMODULO 23\n"<<endl;
		cout<<"Por favor, escoja una opcion: \n"<<endl;              //menu que sirve para el inicio de sesion, registrarse y salir de la aplicacion
		cout<<"1. Iniciar sesion"<<endl;
		cout<<"2. Registrarse"<<endl;
		cout<<"0. Salir de la aplicacion\n"<<endl;
		cout<<"->";
		cin>>opMenu1;
		fflush(stdin);
		switch(opMenu1){
			case 0:
				cout<<"\nHasta luego...\n"<<endl;           // si el ususrio selecciona 0 en dicho menu, sale del programa
				break;	
			case 1:
				clrscr();
				cout<<"\t\tINICIO DE SESION\n"<<endl;
				if (!existeLista(users)){
					cout<<"No hay usuarios registrados. Por favor, pulse ENTER y registrese."<<endl;
					cin.get();
					fflush(stdin);				
				}
				else{                                                                        // estas simplemente son validaciones de si ya se registro
					cout<<"Por favor, ingrese los datos solicitados: \n"<<endl;				// un usuario, si ya existe una cedula registrada, verifica la clave si la 
					cout<<"Cedula: "; cin>>cedula; fflush(stdin);						   // cedula existe, todo al momento de ingresar.
					if (!verificarCedula(users,cedula)){
						cout<<"\nLa cedula ingresada no esta registrada. Pulse ENTER para continuar."<<endl;
						cin.get();
						fflush(stdin);
					}
					else{
						cout<<"Clave: "; getline(cin,clave);
						if (!verificarClave(users,cedula,clave)){
							cout<<"\nLa clave es incorrecta. Pulse ENTER para continuar."<<endl;
							cin.get();
							fflush(stdin);	
						}
						else{
							buscarUsuarioActual(users,cedula);			// funcion que busca el usuario actual en base a la cedula ingresada ente la lista de usuarios
						    if (usuarioActual->tipoUsuario == "D"){      
						    	do{
						    		clrscr();							// aqui entra el tipo de usuario Docente
						    		cout<<"\t\tDOCENTE\n"<<endl;
						    		cout<<"Bienvenido(a): "<<usuarioActual->nombres<<" "<<usuarioActual->apellidos<<endl;
						    		cout<<"\nPor favor, escoja una opcion: \n"<<endl;
						    		cout<<"1. Agregar pregunta al banco de preguntas"<<endl;
						    		cout<<"2. Modificar pregunta del banco de preguntas"<<endl;
						    		cout<<"3. Eliminar pregunta del banco de preguntas"<<endl;
						    		cout<<"4. Crear examen"<<endl;
						    		cout<<"5. Eliminar examen"<<endl;                                                       // menu de docente
						    		cout<<"6. Modificar examen"<<endl;
						    		cout<<"7. Mostrar lista de examenes"<<endl;
						    		cout<<"8. Mostrar promedio de notas y tiempo de duracion de los examenes presentados"<<endl;
						    		cout<<"9. Mostrar alumnos aprobados en un examen"<<endl;
						    		cout<<"10. Mostrar datos de todos los examenes presentados por un estudiante"<<endl;
						    		cout<<"11. Mostrar lista de examenes presentados"<<endl;
						    		cout<<"0. Cerrar sesion"<<endl;
						    		cout<<"\n->"; cin>>opMenuDocente; fflush(stdin);
						    		srand(time(NULL));
						    		switch(opMenuDocente){                        // con este switch determinamos las opciones en el menu de docente
						    			case 0:
						    				break;
						    				
						    			case 1:
						    				clrscr();                                 
						    				cout<<"\t\tCREAR PREGUNTA\n"<<endl;         
						    				
						    				do{										// este do-while crea un codigo de pregunta y verifica que no exista una pregunta con el mismo codigo
						    					codigoPregunta = (rand()%1000)+1;            
											}while(existeCodigoPregunta(bancoPreguntas,codigoPregunta));
												
						    				cout<<"Ingrese el enunciado de su pregunta: "; getline(cin,enunciadoPregunta); // se lee el enunciado de la pregunta
						    				do{
						    					cout<<"Ingrese el tipo de pregunta (SM = Seleccion Multiple | VF = Verdadero o Falso): "; getline(cin,tipoPregunta);  
												tipoPregunta = mayuscula(tipoPregunta);	
						    					if(!preguntaCorrecta(tipoPregunta)){
						    						cout << "\nDebe ingresar un tipo de pregunta correcto: "<<endl;  // validacion de que ingrese un tipoo de pregunta correcta y se repite hasta
												}																	// que se cumpla la condicion		
											}while(!preguntaCorrecta(tipoPregunta));
						    				
						    				existeRespCorrecta = false;
						    				if (tipoPregunta == "SM"){
												do{
													cout<<"Ingrese la cantidad de respuestas posibles: "; 
													cin>>cantidadRespuestas; fflush(stdin);
													if(cantidadRespuestas<2){
														cout<<"Como es seleccion multiple, la pregunta debe tener minimo 2 respuestas.\n"<<endl;
													}
													else{                                                                              // validamos que en seleccion multiple hayan al menos dos posibles respuestas                   
														if(cantidadRespuestas>5){													  //minimo y 5 respuestas posibles maximo
															cout<<"La pregunta no puede tener mas de 5 respuestas posibles.\n"<<endl;
														}
													}
												}while((cantidadRespuestas<2)||(cantidadRespuestas>5));
						    					for (int contadorRespuestas = 1; contadorRespuestas<=cantidadRespuestas; contadorRespuestas++){
						    						cout<<"\nIngrese la opcion "<<contadorRespuestas<<": "; getline(cin, respuestaPosible);
						    						do{
						    							cout<<"La opcion es correcta? (s/n): "; getline(cin, respuestaCorrecta);
						    							respuestaCorrecta = mayuscula(respuestaCorrecta);
						    							if(respuestaCorrecta=="S"){
						    								existeRespCorrecta = true;
						    								contRespCorrectas++;
														}
														if(!verResp(respuestaCorrecta)){
															cout << "\nDebe ingresar 's' o 'n' "<<endl;
														}	
													}while(!verResp(respuestaCorrecta));                          // aqui se verifica que el docente ingrese S o N o s o n
						    						agregarRespuesta(listaResp, contadorRespuestas, respuestaPosible, respuestaCorrecta);
												}
											}
											else{
												cantidadRespuestas = 1;
												do{
													cout<<"El enunciado es verdadero o falso? (V/F): "; getline(cin,respuestaPosible);	
													respuestaPosible =mayuscula(respuestaPosible);
													if(!verVerdaderoFalso(respuestaPosible)){
														cout <<"Debe ingresar V o F"<<endl;
													}	
												}while(!verVerdaderoFalso(respuestaPosible));
												existeRespCorrecta = true;
												respuestaCorrecta = "S";
												contRespCorrectas=1;
												agregarRespuesta(listaResp,1,respuestaPosible,respuestaCorrecta);
											}
											if(existeRespCorrecta == true){ //verificamos que haya ingresado al menos una respuesta correcta
												agregarPregunta(bancoPreguntas, listaResp, enunciadoPregunta, tipoPregunta, codigoPregunta,cantidadRespuestas, contRespCorrectas);
												contRespCorrectas=0;
												cout<<"\nLa pregunta ha sido agregada exitosamente."<<endl;
												archivarPreguntas(bancoPreguntas);
												cout<<"A continuacion se muestra el nuevo banco de preguntas: "<<endl;
												mostrarListaPreguntas(bancoPreguntas);
												cout<<"\nPulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
												listaResp = NULL;
											}
											else{
												//dejamos vacia la lista de respuestas creadas ya que no se asigno a ninguna pregunta
												contRespCorrectas = 0;
												while(listaResp!=NULL){
													auxListaResp = listaResp;
													listaResp = auxListaResp->siguiente;
													delete auxListaResp;
												}
												auxListaResp = NULL;
												cout<<"\nNo se pudo crear la pregunta ya que no asigno respuestas correctas. Pulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
										
										case 2:
											clrscr();
											cout<<"\t\tMODIFICAR PREGUNTA\n"<<endl;       
											contRespCorrectas=0;
											existeRespCorrecta = false;
											if (!existeBancoPreguntas(bancoPreguntas)){
												cout<<"No existe un banco de preguntas todavia."<<endl;     // antes de modificar verificamos que exista un banco de preguntas
												cout<<"Pulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												mostrarListaPreguntas(bancoPreguntas);
												cout<<"\nPor favor, ingrese el codigo de la pregunta a modificar: ";
												do{
													cin>>codigoPregunta; fflush(stdin);
													if(!existeCodigoPregunta(bancoPreguntas,codigoPregunta)){          //verificacion del codigo de la pregunta a modificar
														cout<<"\nPor favor, ingrese un codigo valido: ";
													}
												}while(!existeCodigoPregunta(bancoPreguntas,codigoPregunta));
												
												if(existePreguntaExamen(listaExamenes,codigoPregunta)){
													cout<<endl<<"Lo siento, no puede modificar la pregunta porque ya esta incluida en un examen"<<endl;
													cout<<"Pulse ENTER para continuar"<<endl;
													cin.get();                                    // verificacion de que la pregunta a modificar este incluida en un examen, ya que si esta incluida
													fflush(stdin);								 // no se puede modificar la pregunta
												}
												else{
													cout<<"Ingrese nuevo enunciado de su pregunta: "; getline(cin,enunciadoPregunta);
						 		   					do{
						   	 							cout<<"Ingrese el tipo de pregunta (SM = Seleccion Multiple | VF = Verdadero o Falso): "; getline(cin,tipoPregunta);
														tipoPregunta = mayuscula(tipoPregunta);		
						    							if(!preguntaCorrecta(tipoPregunta)){
						    								cout <<"\nDebe ingresar un tipo de pregunta correcto: "<<endl;
														}
													}while(!preguntaCorrecta(tipoPregunta));
						    						if (tipoPregunta == "SM"){
						    							do{  																// y aqui modificamos la pregunta usando el mismo codigo de la pregunta anterior.
															cout<<"Ingrese la cantidad de respuestas posibles: ";           //esta funcion se explica en la libreria      
															cin>>cantidadRespuestas; fflush(stdin);
															if(cantidadRespuestas<2){
																cout<<"Como es seleccion multiple, la pregunta debe tener minimo 2 respuestas.\n"<<endl;
															}
															else{
																if(cantidadRespuestas>5){
																	cout<<"La pregunta no puede tener mas de 5 respuestas posibles.\n"<<endl;
																}
															}
														}while((cantidadRespuestas<2)||(cantidadRespuestas>5));
						    							for (int contadorRespuestas = 1; contadorRespuestas<=cantidadRespuestas; contadorRespuestas++){
						    								cout << "\nIngrese la opcion "<<contadorRespuestas<<": ";
						    								getline(cin,respuestaPosible);
						    								do{
						    									cout<<"La opcion es correcta? (s/n): "; getline(cin, respuestaCorrecta);
						    									respuestaCorrecta = mayuscula(respuestaCorrecta);
						    									if(respuestaCorrecta =="S"){
						    										existeRespCorrecta = true;                                             
						    										contRespCorrectas++;
																}
																if(!verResp(respuestaCorrecta)){
																	cout << "\nDebe ingresar 's' o 'n' "<<endl;
																}	
															}while(!verResp(respuestaCorrecta));
						    								agregarRespuesta(listaResp,contadorRespuestas,respuestaPosible,respuestaCorrecta);
														}
													}
													else{
														cantidadRespuestas = 1;
														do{
															cout<<"El enunciado es verdadero o falso? (V/F): "; getline(cin,respuestaPosible);	
															respuestaPosible =mayuscula(respuestaPosible);
															if(!verVerdaderoFalso(respuestaPosible)){
																cout <<"Debe ingresar V o F"<<endl;
															}	
														}while(!verVerdaderoFalso(respuestaPosible));
														existeRespCorrecta = true;
														respuestaCorrecta = "S";
														contRespCorrectas = 1;
														agregarRespuesta(listaResp,1,respuestaPosible,respuestaCorrecta);
													}
													if(existeRespCorrecta==true){
														cout<<"\nLa pregunta se ha modificado exitosamente."<<endl;
														modificarPregunta(bancoPreguntas,codigoPregunta,listaResp,enunciadoPregunta,tipoPregunta,contRespCorrectas);
														listaResp = NULL;
														archivarPreguntas(bancoPreguntas);
														cout<<"\nLa nueva pregunta con los datos modificados es: \n"<<endl;
                           		                 		mostrarPregunta(bancoPreguntas,codigoPregunta);
                               		             		contRespCorrectas=0;
                                   		         		cin.get();
                                       		     		fflush(stdin);
                                       		     	}
                                       		     	else{
                                       		     		while(listaResp!=NULL){
                                       		     			auxListaResp = listaResp;
															listaResp = auxListaResp->siguiente;
															delete auxListaResp;	
														}
														auxListaResp = NULL;
														contRespCorrectas = 0;	
                                       		     		cout<<"\nLa pregunta no se pudo modificar ya que no asigno respuestas correctas. Pulse ENTER para continuar."<<endl;
                                       		     		cin.get();
                                       		     		fflush(stdin);
													}
                                                }
											}
											break;
											
										case 3:
											clrscr();
											cout<<"\t\tELIMINAR PREGUNTA\n"<<endl;
											if(!existeBancoPreguntas(bancoPreguntas)){                             //el docente tiene la posibilidad de eliminar una pregunta creada en el banco de preguntas
												cout<<"No existe un banco de preguntas todavia."<<endl;			  // siempre y cuando ingrese el codigo de una pregunta que existe en el banco de preguntas
												cout<<"Pulse ENTER para continuar"<<endl;                        // y una vez validado el codigo la pregunta no puede estar incluida en ningun examen
												cin.get();
												fflush(stdin);
											}
											else{
												mostrarListaPreguntas(bancoPreguntas);
												cout<<"\nPor favor, ingrese el codigo de la pregunta a eliminar: ";
												do{
													cin>>codigoPregunta; fflush(stdin); 
													if(!existeCodigoPregunta(bancoPreguntas,codigoPregunta)){
														cout<<"\nPor favor, ingrese un codigo valido: ";
													}
												}while(!existeCodigoPregunta(bancoPreguntas,codigoPregunta));
																														// si la pregunta ya esta incluida en un examen no se puede eliminar
												if(existePreguntaExamen(listaExamenes,codigoPregunta)){          
													cout<<endl<<"Lo siento, no puede eliminar la pregunta porque ya esta incluida en un examen"<<endl;
													cout<<"Pulse ENTER para continuar"<<endl;
													cin.get();
													fflush(stdin);
												}
												else{
													eliminarPregunta(bancoPreguntas,codigoPregunta);                 // procedimiento que elimina una pregunta del banco de preguntas
													cout << "---------------------------------------"<<endl;
													cout << "PREGUNTA ELIMINADA CORRECTAMENTE. Pulse ENTER para continuar."<<endl;
													archivarPreguntas(bancoPreguntas);
													cin.get();
													fflush(stdin);
										     	}
											}
											break;
											
										case 4:	
											clrscr();
											cout<<"\t\tCREAR EXAMEN\n"<<endl;							// aqui comienza el crear examen
											if(existeBancoPreguntas(bancoPreguntas)==false){
												cout<<"Debe crear un banco de preguntas primero. Pulse ENTER para volver."<<endl;
												cin.get();		
												fflush(stdin);					//verifica si existe un banco de preguntas
											}
											else{
												do{
													auxCantPreguntas = 0;
													cout<<"Por favor, ingrese la cantidad de preguntas del examen: "; cin>>cantidadPreguntas; fflush(stdin);
													if(cantidadPreguntas<0){
														cout<<"Debe ingresar una cantidad valida"<<endl<<endl;
													}
													else{
														listaPreguntas = bancoPreguntas;
														while(listaPreguntas!=NULL){
															auxCantPreguntas++;
															listaPreguntas = listaPreguntas->siguiente;        // aqui verifica si la cantidad de preguntas ingresadas para el examen es mayor que la cantidad del banco de preguntas
														}
														if(cantidadPreguntas > auxCantPreguntas){
															cout<<"El banco de preguntas no posee tal cantidad de preguntas"<<endl<<endl;
														}
													}
												}while((cantidadPreguntas > auxCantPreguntas)||(cantidadPreguntas<0));
												listaPreguntas = NULL;
												mostrarListaPreguntas(bancoPreguntas);
												cout<<"\n\nIngrese los codigos de las preguntas para el examen: \n"<<endl;
												for(int contador=1;contador<=cantidadPreguntas;contador++){
													do{
														cout<<"Pregunta "<<contador<<": "; cin>>codigoPregunta; fflush(stdin);
														validarOpcion =noRepetida(listaOpciones,codigoPregunta);
														if(existeCodigoPregunta(bancoPreguntas,codigoPregunta)==false){
															cout<<"Debe ingresar una opcion correcta.\n "<<endl;                  // aqui verificamos que haya ingresado un codigo de pregunta correcto y que no este ingresada anteriormente
														}
														else{
															if(validarOpcion==true){
																cout<<"La pregunta ya se encuentra ingresada en el examen.\n"<<endl;
															}
														}
													}while((existeCodigoPregunta(bancoPreguntas,codigoPregunta)==false)||(validarOpcion==true));	
											
													agregarPregExamen(listaPreguntas,bancoPreguntas,codigoPregunta);
												}
												respuestaOpcion *auxOpcion;
												while(listaOpciones!=NULL){
													auxOpcion = listaOpciones;
													listaOpciones = auxOpcion->proximo;
													delete auxOpcion;	
												}
												cout<<"\nIngrese el titulo del examen: "; getline(cin,tituloExamen);
												srand(time(NULL));
												do{
													codigoExam = 1 + rand()%1000;	
												}while(existeCodigoExamen(listaExamenes,codigoExam));
											 
												agregarExamen(listaExamenes,codigoExam,tituloExamen,cantidadPreguntas,listaPreguntas);     // agrega el examen a la lista de examenes
												archivarExamen(listaExamenes);
												listaPreguntas = NULL;
												cout<<"\n\n";
												cout<<"El examen ha sido creado correctamente. Pulse ENTER para ver los datos del examen creado."<<endl;
												cin.get();
												fflush(stdin);
												mostrarExamenCreado(listaExamenes,codigoExam);
												cout<<"\nPulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
										
										case 5:
											clrscr();
											cout<<"\t\tELIMINAR EXAMEN\n"<<endl;                            // aqui elimina un examen siempre y cuando no haya sido presentado por un estudiante
											if(existeListaExamenes(listaExamenes)==false){ 
												cout<<"No existe una lista de examenes todavia. Pulse ENTER para volver"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para ver la lista de examenes.\n"<<endl;
												cin.get();
												fflush(stdin);
												mostrarListaExamenes(listaExamenes);
												cout<<"\n\nPor favor, ingrese el codigo del examen a eliminar: "; 
												do{
													cin>>codigoExam; fflush(stdin);
													if(!existeCodigoExamen(listaExamenes,codigoExam)){
														cout<<"\nDebe ingresar una opcion correcta: ";
													}
												}while(!existeCodigoExamen(listaExamenes,codigoExam));                       // aqui valida que el codigo del exmane exista en la lista de examenes
											
												if (verificarExamenPresentado(listaExamenesPresentados,codigoExam)){
													cout<<endl<<"Lo siento el examen ya lo han presentado, no se puede eliminar"<<endl;
													cout<<"Pulse ENTER para continuar"<<endl;
													cin.get();
													fflush(stdin);
												}
												else{
													eliminarExamen(listaExamenes,codigoExam);
													archivarExamen(listaExamenes);
													cout<<"Examen eliminado correctamente. Pulse ENTER para volver."<<endl;
													cin.get();
													fflush(stdin);
												}
									   		}
											break;
										
										case 6:
											clrscr();
											cout<<"\t\tMODIFICAR EXAMEN\n"<<endl;                     // aqui se modifica un examen siempre y cuando no haya sido presentado por un estudiante
											if(existeListaExamenes(listaExamenes)==false){
												cout<<"No existe una lista de examenes todavia. Pulse ENTER para volver"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para ver la lista de examenes.\n"<<endl;
												cin.get();
												fflush(stdin);
												mostrarListaExamenes(listaExamenes);
												do{
													cout<<"\n\nPor favor, ingrese el codigo del examen a modificar: ";cin>>codigoExam;fflush(stdin);
													if (!existeCodigoExamen(listaExamenes,codigoExam)){
														cout<<"POR FAVOR INGRESE UN CODIGO CORRECTO"<<endl;
													}
												}while(!existeCodigoExamen(listaExamenes,codigoExam));                      // verifica que el codigo ingresado exista
											
												if (verificarExamenPresentado(listaExamenesPresentados,codigoExam)){
													cout<<endl<<"Lo siento el examen ya lo han presentado, no se puede modificar"<<endl;
													cout<<"Pulse ENTER para continuar"<<endl;                                              // si ya lo presento un estudiante no se puede modificar
													cin.get();
													fflush(stdin);
												}
												else{
													cout<<"\n\nPor favor ingrese el nuevo titulo del examen:  ";getline(cin,tituloExamen);
													cout<<"Por favor, ingrese la nueva cantidad de preguntas del examen: "; cin>>cantidadPreguntas; fflush(stdin);
													mostrarListaPreguntas(bancoPreguntas);
													cout<<"\n\nIngrese los codigos de las nuevas preguntas para el examen: \n"<<endl;
													for(int contador=1;contador<=cantidadPreguntas;contador++){
														do{	
															cout<<"Pregunta "<<contador<<": "; cin>>codigoPregunta; fflush(stdin);
															if(!existeCodigoPregunta(bancoPreguntas,codigoPregunta)){
																cout<<"POR FAVOR INGRESE UN CODIGO DE PREGUNTA CORRECTO"<<endl;
															}
											    		}while(!existeCodigoPregunta(bancoPreguntas,codigoPregunta));
														agregarPregExamen(listaPreguntas,bancoPreguntas,codigoPregunta);
													}
													modificarExamen(listaExamenes,codigoExam,tituloExamen,cantidadPreguntas,listaPreguntas);
													archivarExamen(listaExamenes);
													listaPreguntas = NULL;
													cout<<"\n\n";
													cout<<"El examen ha sido modificado correctamente. Pulse ENTER para ver los datos del examen modificado."<<endl;
													cin.get();
													fflush(stdin);
													mostrarExamenCreado(listaExamenes,codigoExam);
													cin.get();
													fflush(stdin);
												}
											}
											break;
										
										case 7:
											clrscr();
											cout<<"\t\tMOSTRAR LISTA DE EXAMENES\n"<<endl;                 // aqui simplemente se muestra la lista de examenes previamente creados
											if(existeListaExamenes(listaExamenes)==false){
												cout<<"No existe una lista de examenes todavia. Pulse ENTER para volver"<<endl;
												cin.get();
											fflush(stdin);
									    	}
											else{
												cout<<"Pulse ENTER para ver la lista de examenes.\n"<<endl;
												cin.get();
												fflush(stdin);
												mostrarListaExamenes(listaExamenes);
												cout<<"\n\nPulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);	
											}
									    	break;
									
										case 8:
											clrscr();
											cout<<"\t\tPROMEDIO DE NOTAS (TODOS LOS EXAMENES)\n"<<endl;
											if(existeListaExamenesPresentados(listaExamenesPresentados) == false){
												cout<<"Los estudiantes no han presentado ningun examen aun. Pulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}                                                                     // aqui se muestra el promedio de notas de todos los examenes presentados por los estudiantes
											else{
												cout<<"Pulse ENTER para ver los promedios."<<endl;
												cin.get();
												fflush(stdin);
												examenActual = listaExamenes;
												while(examenActual!=NULL){
													cout<<"**********************************"<<endl;
													cout<<"Codigo de examen: "<<examenActual->codigo<<endl;
													cout<<"Titulo de examen: "<<examenActual->titulo<<endl;
													if(verificarExamenPresentado(listaExamenesPresentados,examenActual->codigo)==true){
														cout<<"Promedio de notas: "<<promedioDeNotasEstudiantes(listaExamenesPresentados,examenActual->codigo)<<endl;
														cout<<"Promedio de duracion: "<<promedioDeDuracionExamenes(listaExamenesPresentados,examenActual->codigo)<<" minutos"<<endl;
													}
													else{
														cout<<"Ningun alumno ha presentado el examen"<<endl;
													}
													cout<<"**********************************\n"<<endl;
													examenActual = examenActual->siguiente;
												}
												cout<<"\nPulse ENTER para volver al menu."<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
									
										case 9:
											clrscr();
											cout<<"\t\tALUMNOS APROBADOS EN UN EXAMEN\n"<<endl;                                                     // aqui muestra los estudiantes aprobados y reprobados de acuerdo con el codigo del examen ingresado
											if(existeListaExamenesPresentados(listaExamenesPresentados) == false){
												cout<<"Los estudiantes no han presentado ningun examen aun. Pulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para ver la lista de examenes."<<endl;
												cin.get();
												fflush(stdin);
												mostrarExamenesSinDatos(listaExamenes);
												cout<<endl;
												do{
													cout<<"\nPor favor, ingrese el codigo de un examen: "; cin>>codigoExam; fflush(stdin);
													if(existeCodigoExamen(listaExamenes,codigoExam)==false){
														cout<<"Debe ingresar una opcion valida."<<endl;
													}
												}while(existeCodigoExamen(listaExamenes,codigoExam)==false);
											
												if(verificarExamenPresentado(listaExamenesPresentados,codigoExam)==false){
													cout<<"\nNingun alumno ha presentado ese examen. Pulse ENTER para continuar."<<endl;
													cin.get();
													fflush(stdin);
												}
												else{
													cout<<"\nA continuacion, las cedulas de los aprobados y los reprobados: \n"<<endl;
													cout<<"****************************";
													cout<<"\n\t\tAPROBADOS\n"<<endl;
													presentado *auxListaPresentados = listaExamenesPresentados; //recorremos la lista de presentados con un auxiliar
													while(auxListaPresentados != NULL){
														if ((auxListaPresentados->codigoExamen == codigoExam)&&(auxListaPresentados->nota >= 10)){
															cout<<auxListaPresentados->cedula<<" con "<<auxListaPresentados->nota<<endl;
														}
														auxListaPresentados = auxListaPresentados->siguiente;
													}
													cout<<"****************************\n"<<endl;
												
													cout<<"****************************";
													cout<<"\n\t\tREPROBADOS\n"<<endl;
													auxListaPresentados = listaExamenesPresentados;
													while(auxListaPresentados != NULL){
														if ((auxListaPresentados->codigoExamen == codigoExam)&&(auxListaPresentados->nota < 10)){
															cout<<auxListaPresentados->cedula<<" con "<<auxListaPresentados->nota<<endl;
														}
														auxListaPresentados = auxListaPresentados->siguiente;
													}
													cout<<"****************************\n"<<endl;
												
													cout<<"Pulse ENTER para volver al menu."<<endl;
													cin.get();
													fflush(stdin);
												}
											}
											break;	
									
										case 10:                        // aqui se mutesran los datos de los examenes presentados por un determinado estudiante
											clrscr();
											cout<<"\t\tDATOS DE LOS EXAMENES PRESENTADOS POR UN ESTUDIANTE\n"<<endl;
											if(existeListaExamenesPresentados(listaExamenesPresentados)==false){
												cout<<"Ningun alumno ha presentado un examen. Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para mostrar la lista de estudiantes."<<endl;
												cin.get();
												fflush(stdin);
												mostrarListaUsuarios(users);
												do{
													cout<<"\nIngrese la cedula de un estudiante: "; cin>>cedula; fflush(stdin);
													if(verificarCedula(users,cedula)==false){
														cout<<"Debe ingresar una cedula valida."<<endl;
													}
												}while(verificarCedula(users,cedula)==false);
											
												if(cedulaPresento(listaExamenesPresentados,cedula)==false){
													cout<<"\nEl usuario no ha presentado ningun examen. Pulse ENTER para continuar"<<endl;
													cin.get();
													fflush(stdin);
												}
												else{
													cout<<"\nLos datos de los examenes presentados por el usuario con cedula "<<cedula<<" son: \n"<<endl;
													mostrarExamenesEstudiante(listaExamenesPresentados,cedula);
													cout<<"\nPulse ENTER para continuar."<<endl;
													cin.get();
													fflush(stdin);
												}
											}
											break;
											
										case 11:                                        // y aqui se muestra la lista de examenes que ya han sido presentados
											clrscr();
											cout<<"\t\tLISTA DE EXAMENES PRESENTADOS\n"<<endl;
											if(existeListaExamenesPresentados(listaExamenesPresentados)==false){
												cout<<"Ningun alumno ha presentado un examen. Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para ver la lista\n"<<endl;
												cin.get();
												fflush(stdin);
												mostrarListaExamenesPresentados(listaExamenesPresentados);
												cout<<"Pulse ENTER para continuar."<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
																							
										default: 
											cout << "Debe ingresar una opcion correcta"<<endl;	
											cin.get();
											fflush(stdin);													
									}
								}while (opMenuDocente != 0);
							}	
							
							if (usuarioActual->tipoUsuario == "E"){ 
								do{                                               // ingresamos al tipo de usuario estudiante
									clrscr();
									cout<<"\t\tESTUDIANTE\n"<<endl;
									cout<<"Bienvenido(a): "<<usuarioActual->nombres<<" "<<usuarioActual->apellidos<<endl;
									cout<<"\nPor favor, escoja una opcion: \n"<<endl;
									cout<<"1. Presentar examen"<<endl;
									cout<<"2. Ver nota de un examen"<<endl;                 // menu del estudiante
									cout<<"3. Ver notas acumuladas"<<endl;
									cout<<"4. Ver promedio de notas acumuladas"<<endl;
									cout<<"5. Ver examen realizado"<<endl; 
									cout<<"0. Cerrar sesion"<<endl;
									cout<<"\n->"; cin>>opMenuEstudiante; fflush(stdin);
									
									switch(opMenuEstudiante){
										case 0:
											break;            // si la opcion es 0 se regresa al menu de registro e inicio de sesion
										
										case 1:
											clrscr();
											tInicial = 0;
											tFinal = 0;
											duracion = 0;
											cout<<"\t\tPRESENTACION DE EXAMEN\n"<<endl;              // aqui entra el estudiante cuando desea presentar un examen y verifica qu haya una lista de examenes creada por el docente
											if(existeListaExamenes(listaExamenes) == false){
												cout<<"No existe una lista de examenes aun. Pulse ENTER para continuar y avise a su docente."<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Pulse ENTER para ver la lista de examenes."<<endl;
												cin.get();
												fflush(stdin);
												mostrarExamenesSinDatos(listaExamenes);
												do{
													cout<<"\nPor favor, ingrese el codigo del examen a presentar: ";
													cin>>codigoExam;                                                   // aqui ingresa el codigo del examen a presentar y verifica que exista
													fflush(stdin);
													if (existeCodigoExamen(listaExamenes,codigoExam) == false){
														cout<<"Debe ingresar una opcion correcta."<<endl;
													}	
												}while(existeCodigoExamen(listaExamenes,codigoExam)==false);
												
												if(cedulaPresentoExamen(listaExamenesPresentados,usuarioActual->cedula,codigoExam)){
													cout <<"Lo sentimos, usted ya presento este examen"<<endl; //verificamos que el examen no haya sido presentado por el mismo anteriormente
													cout<<"Pulse ENTER para volver al menu"<<endl;
													cin.get();
													fflush(stdin);
												}
												else{
													//buscamos el examen que el estudiante escogio para presentar
													examenActual=NULL;
													buscarExamenActual(listaExamenes,codigoExam);
													//creamos la lista de preguntas que el estudiante va a responder, solo para despues asignarles sus respectivas respuestas.
													// Solo servira para la opcion numero 5 del menu estudiante
													//ya que tenemos que mostrarle las respuestas que el dio
													agregarPreguntasPresentado(listaPreguntasPresentado,examenActual); 
													cout<<"\nVa a presentar el examen: "<<examenActual->titulo<<endl;
													cout<<"Puntaje por pregunta: "<<examenActual->puntPorPregunta<<endl;
													cout<<"\nPulse ENTER para empezar."<<endl;
													cin.get();
													fflush(stdin);
													listaPreguntas = examenActual->preguntasExamen;
													
													now = time(0);
													tiempo = localtime(&now);
													auxHora = tiempo->tm_hour;
													auxMinutos = tiempo->tm_min;
													hora = enteroString(auxHora);											
													minutos = enteroString(auxMinutos);																								
													horaInicio = hora+":"+minutos;
												
													tInicial = tiempoActual();
													contPreguntas = 1;
													notaExamen = 0;
													
													//empieza el examen
													//usamos la listaPreguntas que tambien la usamos para asignar la lista de preguntas a cada examen
													//ya que lo podemos considerar como un auxiliar que se puede reutilizar aqui
													while(listaPreguntas!=NULL){
														clrscr();
														notaActual = 0;
														auxCantRespEst = 0;
														listaResp = listaPreguntas->respuestasPosibles;
														cout<<"\t\t"<<examenActual->titulo<<"\n"<<endl;
														cout<<"Pregunta "<<contPreguntas<<": ";
														cout<<listaPreguntas->enunciado<<"\n"<<endl;
														while(listaResp!=NULL){
															cout<<listaResp->codigo<<". "<<listaResp->respuestaPosible<<endl;
															listaResp = listaResp->siguiente;
														}
														if(listaPreguntas->tipo == "VF"){
															if(listaPreguntas->respuestasPosibles->respuestaPosible == "V"){
																cout<<"2. F"<<endl;
															}
															else{
																cout<<"2. V"<<endl;
															}
														}
														cout<<"0. Pasar a la siguiente"<<endl;
														if(listaPreguntas->tipo == "SM"){
															cout<<"\nNOTA: Podra escoger una o mas respuestas."<<endl;
														}
														cout<<endl;
														listaResp = listaPreguntas->respuestasPosibles;
														int opcionRespuesta = -1, contador=1;  
														do{
															//la igualamos al principio del ciclo ya que va a cambiar para hacer comparaciones 
															listaResp = listaPreguntas->respuestasPosibles;
															do{
																cout<<"Ingrese numero de respuesta: "; cin>>opcionRespuesta; fflush(stdin);
																if(((listaPreguntas->tipo == "SM")&&((opcionRespuesta<0)||(opcionRespuesta>listaPreguntas->numeroResp))) || ((listaPreguntas->tipo == "VF")&&((opcionRespuesta<0)||(opcionRespuesta>2)))){
																	cout<<"Debe ingresar una opcion valida\n"<<endl;
																}
															}while(((listaPreguntas->tipo == "SM")&&((opcionRespuesta<0)||(opcionRespuesta>listaPreguntas->numeroResp))) || ((listaPreguntas->tipo == "VF")&&((opcionRespuesta<0)||(opcionRespuesta>2))));
															
															
															
															if(opcionRespuesta!=0){
																auxCantRespEst++;
																
																if(listaPreguntas->tipo=="SM"){
																	while(listaResp->codigo != opcionRespuesta){
																		listaResp = listaResp->siguiente;
																	}	
																	validarOpcion = noRepetida(listaOpciones,opcionRespuesta);
																	if(validarOpcion == true){
																		cout << "Esta opcion ya fue ingresada anteriormente\n"<<endl;
																		contador--;
																		auxCantRespEst--;
																	}
																	else{
																		//aqui esta el factor de correccion de las preguntas de tipo SM
																		//si es correcta la opcion, se le suma el puntaje de la pregunta dividido entre la cantidad de respuestas correctas que el docente establecio
																		agregarRespuestasPreguntas(listaPreguntasPresentado,opcionRespuesta,listaPreguntas->codigo);
																		if(listaResp->respuestaCorrecta == "S"){
																			notaActual = notaActual + (examenActual->puntPorPregunta/listaPreguntas->cantRespuestasCorrectas);
																		}
																		else{
																			//si no es correcta, se resta a la nota actual (de la pregunta, no del examen)
																			notaActual = notaActual - (examenActual->puntPorPregunta/listaPreguntas->cantRespuestasCorrectas);
																		}	
																	}																
																}
																else{ //si es verdadero y falso la pregunta solo guardamos una respuesta posible y es la correcta.
																	//colocamos validarOpcion aqui tambien para que nos agregue la opcion a la lista de respuestas y no genere errores
																	validarOpcion = noRepetida(listaOpciones,opcionRespuesta);
																	agregarRespuestasPreguntas(listaPreguntasPresentado,opcionRespuesta,listaPreguntas->codigo);
																	if(listaResp->codigo == opcionRespuesta){
																		notaActual = notaActual + examenActual->puntPorPregunta;
																	}
																	opcionRespuesta = 0;
																	//como las preguntas VF no tienen factor de correccion, si la opcion del usuario es incorrecta no suma ni resta nada
																}
																contador++; //contador de ingresar respuestas
															}
															else{
																if((listaOpciones == NULL)&&(auxCantRespEst==0)){ //significa que paso de una vez a la siguiente pregunta
																	preguntasEx *auxiliarNULL = listaPreguntasPresentado;
																	while(auxiliarNULL!=NULL){
																		if(auxiliarNULL->codigoPreg == listaPreguntas->codigo){
																			auxiliarNULL->respuestasEstudiante = NULL;
																		}
																		auxiliarNULL = auxiliarNULL->siguiente;
																	}
																}
															}
														}while((opcionRespuesta!=0)&&(contador<=listaPreguntas->numeroResp));
														
														agregarCantidadRespuestasEstudiante(listaPreguntasPresentado,listaPreguntas->codigo,auxCantRespEst);
														
														//borramos la lista de opciones que usamos para validar que haya ingresado la misma respuesta dos veces
														//para volver a validar en la siguiente pregunta
														respuestaOpcion *auxResp1;
														while(listaOpciones!=NULL){
															auxResp1 = listaOpciones;
															listaOpciones = auxResp1->proximo;
															delete auxResp1;	
														}
														listaResp = listaPreguntas->respuestasPosibles;
														
														//si la nota de la pregunta es negativa, significa que respondio mas respuestas incorrectas que correctas, y la nota de la pregunta es 0
														if(notaActual < 0){
															notaActual = 0;
														}
														notaExamen = notaExamen + notaActual;
														cout<<"\nLLeva acumulado: "<<notaExamen<<" de 20\n"<<endl;
														if(listaPreguntas->tipo == "VF"){
															cout<<"La respuesta correcta era la opcion 1."<<endl;
														}
														else{
															cout<<"Las respuestas correctas eran las opciones: ";
															while(listaResp!=NULL){
																if(listaResp->respuestaCorrecta == "S"){
																	if((listaResp->siguiente != NULL)&&(listaResp->siguiente->respuestaCorrecta == "S")){
																		cout<<listaResp->codigo<<", ";
																	}
																	else{
																		cout<<listaResp->codigo;
																	}
																}
																listaResp = listaResp->siguiente;
															}
														}
														listaPreguntas = listaPreguntas->siguiente;	
														contPreguntas++;
														cout<<"\nPulse ENTER para continuar."<<endl;
														cin.get();
														fflush(stdin);
													}//termina el examen
											   
													tFinal = tiempoActual();
													duracion = (tFinal - tInicial)/60; //pasamos de segundos a minutos
													
													//pedimos el tiempo actual usando la libreria time.h
													now = time(0);
													tiempo = localtime(&now);
													auxHora = tiempo->tm_hour;
													auxMinutos = tiempo->tm_min;
													hora = enteroString(auxHora);
													minutos = enteroString(auxMinutos);																								
													horaFinal = hora+":"+minutos; //concatenamos los datos de la hora
												
													auxDia = tiempo->tm_mday;
													auxMes = 1 + tiempo->tm_mon;
													auxAnio = 1900 + tiempo->tm_year;
													dia = enteroString(auxDia);
													mes = enteroString(auxMes);
													anio = enteroString(auxAnio);
													fechaExamen = dia+"/"+mes+"/"+anio; //concatenamos los datos de la fecha
												
													clrscr();
													cout<<"\t\t"<<examenActual->titulo<<"\n"<<endl;
													cout<<"El examen ha terminado. Datos de la presentacion: \n"<<endl;
													cout<<"Nota: "<<notaExamen<<" de 20"<<endl;
													cout<<"Duracion: "<<duracion<<" minutos"<<endl;
													cout<<"Hora de inicio: "<<horaInicio<<endl;
													cout<<"Hora al finalizar: "<<horaFinal<<endl;
													cout<<"Fecha de presentacion: "<<fechaExamen<<endl;
													cout<<"\nPulse ENTER para volver al menu."<<endl;
												
													agregarPresentado(listaExamenesPresentados,examenActual->codigo,usuarioActual->cedula,notaExamen,duracion,horaInicio,horaFinal,fechaExamen,listaPreguntasPresentado);
													//igualamos la lista de preguntas del examen presentado a NULL ya que se la asignamos al nodo correspondiente del alumno que presento
													listaPreguntasPresentado = NULL;
													
													guardarExamenesPresentados(listaExamenesPresentados);
													cin.get();
													fflush(stdin);
												}
											}
											break;
											
										case 2:
											clrscr();                                     // aqui le muestra al estudiante la nota de un determiando examen
											cout<<"\t\tVER NOTA DE UN EXAMEN\n"<<endl;
											if(!cedulaPresento(listaExamenesPresentados,usuarioActual->cedula)){
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nUsted no ha presentado ningun examen hasta el momento"<<endl;
												cout<<"Pulse ENTER para continuar <>"<<endl;
												cin.get();
												fflush(stdin);
											}
											
											else{
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl<<endl;
												mostrarCodigosExamenesPresentadosEstudiante(listaExamenesPresentados,usuarioActual->cedula);
												cout<<endl<<endl;
												do{
													cout<<"\nPor favor ingrese el codigo del examen que realizo y desea ver su nota"<<endl;cin>>codigoExam;fflush(stdin);
													if(!cedulaPresentoExamen(listaExamenesPresentados,usuarioActual->cedula,codigoExam)){
														cout<<"Ingrese un codigo de examen valido\n"<<endl;
													}
												}while (!cedulaPresentoExamen(listaExamenesPresentados,usuarioActual->cedula,codigoExam));
												mostrarNotaEstudiante(listaExamenesPresentados,usuarioActual->cedula,codigoExam);
												cout<<"Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
										    }
										break;
										
										case 3:
											clrscr();                             // aqui le muestra las notas acumuladas de todos los examenes al estudiante que ingreso
											cout<<"\t\tNOTAS ACUMULADAS\n"<<endl;
											if(!cedulaPresento(listaExamenesPresentados,usuarioActual->cedula)){
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nUsted no ha presentado ningun examen hasta el momento"<<endl;
												cout<<"Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nSus notas acumuladas son:  "<<endl<<endl;
												mostrarNotasEstudiante(listaExamenesPresentados,usuarioActual->cedula);
												cout<<"Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
										
										case 4:
											clrscr();  // aqui le muestra el promedio al estudiante en base a todos los examenes que ha presentado
											cout<<"\t\tPROMEDIO DE NOTAS DE EXAMENES REALIZADOS\n"<<endl;
											if(!cedulaPresento(listaExamenesPresentados,usuarioActual->cedula)){
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nUsted no ha presentado ningun examen hasta el momento"<<endl;
												cout<<"Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nLos examenes que ha realizado son: \n"<<endl;
												mostrarNotasEstudiante(listaExamenesPresentados, usuarioActual->cedula);
												cout<<"\nY su promedio de notas es: "<<promedioDeNotasEstudiante(listaExamenesPresentados,usuarioActual->cedula)<<endl;
												cout<<"\nPulse ENTER para volver al menu"<<endl;
												cin.get();
												fflush(stdin);
											}
											break;
										
										case 5:                // y este muestra todas las respuestas que el estudiante ingreso en un determinado examen y las compara con las respuestas posibles de la pregunta en el examen
											clrscr();													// es decir le muestra las respuestas correctas de cada pregunta y muestra las que el estudiante respondio
											cout<<"\t\tDATOS DE EXAMEN REALIZADO\n"<<endl;
											if(!cedulaPresento(listaExamenesPresentados,usuarioActual->cedula)){
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl;
												cout<<"\nUsted no ha presentado ningun examen hasta el momento"<<endl;
												cout<<"Pulse ENTER para continuar"<<endl;
												cin.get();
												fflush(stdin);
											}
											else{
												cout<<"Estudiante:  "<<usuarioActual->apellidos<<", "<<usuarioActual->nombres<<endl<<endl;
												mostrarNotasEstudiante(listaExamenesPresentados,usuarioActual->cedula);
												do{
													cout<<"\nIngrese el codigo de un examen: "; cin>>codigoExam; fflush(stdin);
													if(existeCodigoExamen(listaExamenes,codigoExam)==false){
														cout<<"Debe ingresar un codigo valido."<<endl;
													}
													else{
														if(cedulaPresentoExamen(listaExamenesPresentados,usuarioActual->cedula,codigoExam)==false){
															cout<<"Usted no ha presentado dicho examen. Por ello no aparece en la lista mostrada."<<endl;
														}
													}
												}while((existeCodigoExamen(listaExamenes,codigoExam)==false)&&(cedulaPresentoExamen(listaExamenesPresentados,usuarioActual->cedula,codigoExam)==false));
												clrscr();
												cout<<"\nLos datos del examen realizado son: \n"<<endl;
												mostrarNotaEstudiante(listaExamenesPresentados,usuarioActual->cedula,codigoExam);
												cout<<"Preguntas y Respuestas: \n"<<endl;
												mostrarPreguntasYRespuestasDelEstudiante(listaExamenesPresentados,usuarioActual->cedula,codigoExam);
												cout<<"\nPulse ENTER para continuar."<<endl;
												listaResp = NULL;
												cin.get();
												fflush(stdin);
											}
											break;
										
										default: 
											cout << "Debe ingresar una opcion correcta"<<endl;	
											cin.get();
											fflush(stdin);		
							    	}				
								}while(opMenuEstudiante != 0);	
						    }
						} 
						
					}
				}
				break;
				
			case 2:               // aqui estamos en la opcion de registrar un usuario
				clrscr();
				cout<<"\t\tREGISTRARSE\n"<<endl;
				cout<<"Por favor, ingrese los datos solicitados: \n"<<endl;
				cout<<"Nombres: ";getline(cin,nombres); 
				cout<<"Apellidos: "; getline(cin,apellidos); 
				cout<<"Cedula: "; cin>>cedula;fflush(stdin);           // aqui lee nombres,apellidos y cedula del usuario a registrar
				if(verificarCedula(users,cedula)==true){
					cout<<"\nLa cedula ingresada ya esta registrada. Pulse ENTER para continuar."<<endl;
					cin.get();
					fflush(stdin);
				}
				else{
					cout<<"Tipo de usuario (D = docente, E = estudiante): ";
					do{
						getline(cin,tipoUsuario);                              // aqui verfica que sea del tipo D o E(docente o estudiante)
						tipoUsuario = mayuscula(tipoUsuario);
						tipoCorrecto = verificarTipo(tipoUsuario);
						if(tipoCorrecto == false){
							cout << "\nDebe de ingresar una opcion valida: ";
						}
					}while(tipoCorrecto!=true);
					if((existeDocente(users)==true)&&(tipoUsuario=="D")){
						cout<<"\nYa existe un docente registrado. Pulse ENTER para volver al menu."<<endl;
						cin.get();
						fflush(stdin);
					}
					else{
                		cout<<"Correo electronico: ";
						do{
							getline(cin,email); 
							emailCorrecto = verificacionEmail(email);         // aqui verifica el correo electronico
							if(emailCorrecto == false){
								cout <<"Ingrese un email valido: ";
							}		
						}while(emailCorrecto!=true);
						cout<<"Clave (minimo 8 caracteres): ";
						do{
							getline(cin,clave);
							contraCorrecta = verificacionContra(clave);       // aqui verifica la clave y que tenga como minimo 8 caracteres
							if(contraCorrecta==false){
								cout <<"La clave debe tener minimo 8 caracteres: ";
							}
						}while(contraCorrecta!=true);
						agregarUsuario(users,nombres,apellidos,cedula,tipoUsuario,email,clave);
						cout<<"\n\t\t----Usuario creado exitosamente----";
						archivarUsuarios(users);
						cout<<"\nPulse ENTER para continuar. "<<endl;
						cin.get();
						fflush(stdin);
					}
				}
				break;
							
			default : 
				cout<<"\nDebe ingresar una opcion correcta. Pulse ENTER para continuar"<<endl;
				cin.get();
				fflush(stdin);
		}
	}while (opMenu1 != 0);
	
	//Borramos las listas creadas para liberar la memoria del heap
	usuario *auxLista1;
	while(users!=NULL){
		auxLista1 = users;
		users = auxLista1->proximo;
		delete auxLista1;
	}
	
	pregunta *auxLista2;
	while(bancoPreguntas!=NULL){
		auxLista2 = bancoPreguntas;
		bancoPreguntas = auxLista2->siguiente;
		delete auxLista2;
	}
	
	examen *auxLista3;
	while(listaExamenes!=NULL){
		auxLista3 = listaExamenes;
		listaExamenes = auxLista3->siguiente;
		delete auxLista3;
	}
	
	presentado *auxLista4;
	while(listaExamenesPresentados!=NULL){
		auxLista4 = listaExamenesPresentados;
		listaExamenesPresentados = auxLista4->siguiente;
		delete auxLista4;
	}
	
	return 0;
}


//Funcion para limpiar la pantalla dependiendo del SO donde se este corriendo
void clrscr(){
    #ifdef _WIN32
        system("cls");
    #else
        system ("clear");
    #endif
}


//Esta funcion me va permitir verificar si un email es correcto
// Donde se comparara si, Tiene @ pero esta no esta ni al principio ni al final, tampoco que se repita
bool verificacionEmail(string email){
	bool verificacion;
	string aux;
	int cont=0,pos,aux2;
	for(int i=0;i<=email.size();i++){
		aux = email[i];
		if(aux =="@"){
			pos = i;
			cont++;
		}
	}
	aux2 = email.size()-1;
	if((pos == 0)||(cont>1)||(cont==0)||(aux2==pos)){
		verificacion = false;
	}else{
		verificacion =true;
	} 
   return verificacion;	
}

//Funcion que verifica si la clave que el usuario ingrese tenga 8 caracteres o mas
bool verificacionContra(string clave){
	int tamano;
	
	tamano = clave.size();
	if(tamano < 8){
		return false;
	}else{
		return true;
	}
}

//Funcion que verifica que el usuario ingrese un tipo de usuario valido
bool verificarTipo(string tipoUsuario){
	if((tipoUsuario!="D")&&(tipoUsuario!="E")){
		return false;
	}else{
		return true;
	}
}

//Al igual que la anterior, verifica que ingrese un tipo valido
bool preguntaCorrecta(string tipoPregunta){
	bool validacion;
	if((tipoPregunta=="SM")||(tipoPregunta =="VF")){
		validacion = true;
	}else{
		validacion = false;
	}
	return validacion;
}

//Al igual que la anterior, verifica que ingrese una opcion valida
bool verResp(string respuestaCorrecta){
	bool validacion;
	if((respuestaCorrecta == "S")|| (respuestaCorrecta == "N")){
		validacion = true;
	}else{
		validacion = false;
	}
	return validacion;
}

//Al igual que la anterior, verifica que ingrese una opcion valida
bool verVerdaderoFalso(string respuestaPosible){
	bool validacion;
	if((respuestaPosible == "V")|| (respuestaPosible == "F")){
		validacion = true;
	}else{
		validacion = false;
	}
	return validacion;
}

//Funcion que usamos para el tipo de usuario, el verdadero y falso, la seleccion de respuesta correcta y el tipo de pregunta
//para garantizar que lo que necesitemos este en mayusculas
string mayuscula(string palabra){
	for(int i=0;i<palabra.size();i++){
		palabra[i] = toupper(palabra[i]);
	}
	return palabra;
}

//Funcion que nos devuelve los segundos transcurridos desde que inicio el programa hasta donde se invoca
//la usamos para saber la duracion exacta de la presentacion de un examen
float tiempoActual(){
	clock_t tiempo;
	float time;
	tiempo = clock();
	time = float(tiempo)/CLOCKS_PER_SEC;
	return time;
}

//Funcion que nos convierte un entero al tipo de dato string
//la usamos para poder concatenar la fecha y las horas en el main para que queden en los formatos
//fecha: DD/MM/AAAA
//hora: HH/MM
string enteroString(int numero){
	stringstream auxiliar;
	string entero;
	
	auxiliar<<numero;
	entero = auxiliar.str();
	return entero;	
}
