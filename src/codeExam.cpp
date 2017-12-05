#include <iostream>
#include <fstream>
#include <queue>
#include <ctime>

using namespace std;


const int N = 12;
int matrizAfinidad[N][N];

void mesaRedonda(bool comensales[], int n, int acumulado, int solucion[], int actual,
	int mejorSolucion[], int &maximoAcumulado, int &nodos);
void mesaRedonda( int solucionFinal[], int &acumuladoFinal, int &nodosVisitados);
void cargarMatriz(string nombreArchivo);

/*	Funci�n de afinidad curiosa que agrupa mucho la afinidad, lo que perjudica las 
	podas por estimaci�n optimista. Adem�s la soluci�n es conocida, pues agrupa los
	valores peque�os alrededor del cero, y los grandes entorno al valor m�ximo  N  	*/

/*int afinidad(int a, int b){
	if(b!=a) return b*a;
	else return 0;
}*/

//Funci�n de afinidad que devuelve el valor cargado en la matrizAfinidad
int afinidad(int i , int j){
	if( i < N && j < N && i!=j){
		return matrizAfinidad[i][j];
	}else return 0;
}


int main(){
	int  mejorSolucion[N], maximoAcumulado = -1, nodos = 0;

	cargarMatriz("Datos1.txt");
	
	int time0 = clock();
	mesaRedonda( mejorSolucion, maximoAcumulado, nodos);

	cout << "	Numero de comendales: " << N << endl;
	cout << "	Tiempo: " << (clock() - time0) << " milisegundos" << endl;
	cout << "	Nodos visitados: " << nodos << endl;
	cout << endl << "	Maximo acumulado: " << maximoAcumulado;

	ofstream fOut;
	fOut.open("Resultado1.txt", ios_base::app);
	fOut << endl << "	Poda: Ninguna, solo factibilidad" << endl;
	fOut << "	Numero de comensales: " << N << endl;
	fOut << "	Tiempo: " << (clock() - time0) << " milisegundos" << endl;
	fOut << "	Nodos visitados: " << nodos << endl;
	fOut << "	Maximo acumulado: " << maximoAcumulado << endl;
	for ( int i = 0; i < N; i ++ ){
		cout << "	" << mejorSolucion[i];
		fOut << "	" << mejorSolucion[i];
	}
	fOut.close();
	while(true){}
}

//Carga una matriz NxN de archivo
void cargarMatriz(string nombreArchivo){
	ifstream fIn;
	fIn.open(nombreArchivo);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fIn >> matrizAfinidad[i][j];
		}
	}
	fIn.close();
}


 //Si ya se ha llenado la mesa estamos ante una solucion
bool esSolucion(int solucion[], int actual, int n){
	return n-1 == actual;
}

//Sobreescribe la solucion dada sobre la antigua
void copiarSolucion(int solucionBuena[], int solucionAntigua[], int tama�o){
	for ( int i = 0; i < tama�o; i++){
		solucionAntigua[i] = solucionBuena[i];
	}
}

/*Llamada inicial que construye lo necesarion para comenzar */
void mesaRedonda( int solucionFinal[], int &acumuladoFinal, int &nodosVisitados){
	bool comensales[N];	//	Array para marcar las personas ya sentadas
	for(int i = 0; i < N; i++)
		comensales[i] = true;

	int solucion[N]; //	Array que lleva la soluci�n que se est� explorando

	solucion[0] = 0;//Sentamos al comensal 0 en la pos 0 para no repetir soluciones
	solucionFinal[0] = 0;
	comensales[0] = false;	

	/* Llamada inicial:
		-comensales: Array que indica los comensales que no han sido colocados. Comienza con el 
	primer comensal ya colocado.
		-n: N�mero de comensales que se van a sentar.
		-acumulado: valor acumulado en la llamada actual, comienza en cero.
		-solucion[]: contiene la posicion de los comensales que se est� probando,
	hasta la posicion actual.
		-actual: indica la posicion de la mesa en la que se est� colocando
	al comensal: comienza en la posicion 1, pues la 0 ya esta determinada.
		-solucionFinal: array donde se guardar� la mejor solucion.
		-acumuladoFinal y nodosVisitados, donde se guardar�n ambos datos */
	
	mesaRedonda(comensales, N, 0, solucion, 1, solucionFinal, acumuladoFinal, nodosVisitados);
}

/*	Sienta al comensal i en la posici�n de la mesa actual, actualizando la mesa */
void sientaPersona(int solucion[], int& acumulado, bool comensales[], int actual, int i){
	solucion[actual] = i;		//Colocamos al comensal 'i' en la posicion de la mesa que toca (actual)
	comensales[i]  = false;		//Marcamos al comensal como sentado

	// Actualizamos la afinidad con la persona que tiene a la izquierda (siempre hay una ya una sentada por la llamada inicial)
	acumulado += afinidad(solucion[actual-1], solucion[actual]);
}

/*	Levanta al comensal i de la posici�n actual	*/
void levantaPersona(int solucion[], int& acumulado, bool comensales[], int actual, int i){
	//Desmarcamos los comensales y el valor acumulado;
	acumulado -= afinidad(solucion[actual-1], solucion[actual]);
	comensales[i] = true;
}

/*	Comprueba que la soluci�n que se va a explorar sea v�lida	 */
bool esValida(bool comensales[], int i){
	//Si el comensal i est� a true, es que a�n no est� sentado y la exploraci�n es factible
	return comensales[i];
}

/*	Indica si la soluci�n actual es mejor que la que ten�amos	*/
bool esMejorSolucion(int maximoAcumulado, int acumulado){
	return (maximoAcumulado < acumulado);
}

/* Debes comenzar con el comensal 0 fijado en la posicion 0 */
void mesaRedonda(bool comensales[], int n, int acumulado, int solucion[], int actual,
						int mejorSolucion[], int &maximoAcumulado, int &nodos){
	nodos++;	//Actualizamos los nodos visitados
	for(int i = 1; i < n; i++){
		if( esValida(comensales, i) )	/*Si el comensal ya est� en la mesa sentado, se pasa al siguiente*/{		

			sientaPersona(solucion, acumulado, comensales, actual, i);//Marcamos como sentado

			if(esSolucion(solucion,actual, n)){		//Si la mesa ya est� llena, tratamos la solucion
				// Para el resultado final actualizamos la afinidad del �ltimo con el primero
				acumulado += afinidad( solucion[actual], solucion[0]); 
				if(esMejorSolucion(maximoAcumulado, acumulado)){	
					//Si la solucion que hemos encontrado es mejor que la que teniamos, la guardamos
					maximoAcumulado = acumulado;
					copiarSolucion(solucion, mejorSolucion, n);
				}
			}
			//Si la mesa aun no est� llena, llamamos de nuevo con la posici�n siguiente
			else	mesaRedonda(comensales, n, acumulado, solucion, actual+1,
								mejorSolucion, maximoAcumulado, nodos);

			levantaPersona(solucion, acumulado, comensales, actual, i);//Levantamos a la persona i de la pos actual
		}
	}
}

/* Esquema general sin poda */
