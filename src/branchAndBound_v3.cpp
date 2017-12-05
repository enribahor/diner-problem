#include <iostream>
#include <fstream>
#include <queue>
#include <ctime>

using namespace std;

const int N = 12;
int matrizAfinidad[N][N];

void mesaRedonda(int matrizMejores[N], int max, bool comensales[], int n, int acumulado, int solucion[], int actual,
	int mejorSolucion[], int &maximoAcumulado, int &nodos);
void mesaRedonda( int solucionFinal[], int &acumuladoFinal, int &nodosVisitados);
void cargarMatriz(string nombreArchivo);



int main(){
	int  mejorSolucion[N], maximoAcumulado = -1, nodos = 0;
	cargarMatriz("Datos1.txt");
	
	int time0 = clock();
	mesaRedonda( mejorSolucion, maximoAcumulado, nodos);

	cout << endl << "	Poda: N maximas afinidades acumuladas y descontandose" << endl;
	cout << "	Numero de comensales: " << N << endl;
	cout << "	Tiempo: " << (clock() - time0) << " milisegundos" << endl;
	cout << "	Nodos visitados: " << nodos << endl;
	cout << "	Maximo acumulado: " << maximoAcumulado;

	ofstream fOut;
	fOut.open("Resultado1.txt", ios_base::app);	
	fOut << endl << "	Poda: N maximas afinidades acumuladas y descontándose" << endl;
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

/*int afinidad(int a, int b){
	if(b!=a)return b*a;
	else return 0;
}*/

int afinidad(int i , int j){
	if( i < N && j < N && i!=j){
		return matrizAfinidad[i][j];
	}else return 0;
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
void copiarSolucion(int solucionBuena[], int solucionAntigua[], int tamaño){
	for ( int i = 0; i < tamaño; i++){
		solucionAntigua[i] = solucionBuena[i];
	}
}

/*Llamada inicial que construye lo necesarion para comenzar */
void mesaRedonda( int solucionFinal[], int &acumuladoFinal, int &nodosVisitados){
	bool comensales[N];
	int solucion[N];

	solucion[0] = 0;//Colocamos inicialmente al comensal 0 en la pos 0 para no repetir
	solucionFinal[0] = 0;
	comensales[0] = false;	

	/*	Creamos un vector con las n mejores afinidades */
	int matrizMejores[N];
	priority_queue<int, vector<int>, less<int> > monticulo;	
	for(int i = 0; i < N; i++){
		for(int j = 0; j <= i; j++)
			monticulo.push(afinidad(i,j));
	}
	/*Sumamos las n afinidades y las acumulamos en un máximo que iremos decrementando*/
	int max = 0;
	for(int j = N-1; j >= 0; j--){	
		matrizMejores[j] = monticulo.top();
		max += matrizMejores[j];
		monticulo.pop();
	}

	/* Llamada inicial:
		-comensales: Array que indica los comensales que no han sido colocados. Comienza con el 
	primer comensal ya colocado.
		-n: Número de comensales.
		-acumulado: valor acumulado en la llamada actual, comienza en cero.
		-solucion[]: contiene la posicion de los comensales que se está probando,
	hasta la posicion actual.
		-actual: indica la posicion de la mesa en la que se está colocando
	al comensal: comienza en la posicion 1, pues la 0 ya esta determinada.
		-solucionFinal: array donde se guardará la mejor solucion.
		-acumuladoFinal y nodosVisitados, donde se guardarán ambos datos
		-max: máxima afinidad, que iremos decrementando usando el vector en el que acumulamos las n 
			mayores, una para cada nivel que estemos recorriendo	*/
	
	mesaRedonda(matrizMejores, max, comensales, N, 0, solucion, 1, solucionFinal, acumuladoFinal, nodosVisitados);
}

void sientaPersona(int matrizMejores[N], int& max, int solucion[], int& acumulado, bool comensales[], int actual, int i){
	solucion[actual] = i;		//Colocamos al comensal 'i' en la posicion de la mesa que toca (actual)
	comensales[i]  = false;		//Marcamos al comensal como sentado

	// Actualizamos la afinidad con la persona que tiene a la izquierda (inicialmente debe haber ya una sentada)
	acumulado += afinidad(solucion[actual-1], solucion[actual]);
	max -= matrizMejores[actual-1];

}

void levantaPersona(int matrizMejores[N], int& max, int solucion[], int& acumulado, bool comensales[], int actual, int i){
	//Desmarcamos los comensales y el valor acumulado;
	acumulado -= afinidad(solucion[actual-1], solucion[actual]);
	comensales[i] = true;
	max += matrizMejores[actual-1];
}

bool esValida(bool comensales[], int i){
	//Si la posicion del comensal está a true, es que aún no está sentado
	return comensales[i];
}

bool esMejorSolucion(int maximoAcumulado, int acumulado){
	return (maximoAcumulado < acumulado);
}

/* Debes comenzar con un comensal fijado en la posicion 0 */
void mesaRedonda(int matrizMejores[N], int max, bool comensales[], int n, int acumulado,
	int solucion[], int actual, int mejorSolucion[], int &maximoAcumulado, int &nodos){
	nodos++;//Actualizamos los nodos visitados
	for(int i = 1; i < n; i++){
		if( esValida(comensales, i) )	/*Si el comensal ya está en la mesa sentado, se pasa al siguiente*/{		

			sientaPersona(matrizMejores, max, solucion, acumulado, comensales, actual, i);//Marcamos como sentado

			if(esSolucion(solucion,actual, n)){		//Si la mesa ya está llena, tratamos la solucion
				// Para el resultado final actualizamos la afinidad del último con el primero
				acumulado += afinidad( solucion[actual], solucion[0]); 
				if(esMejorSolucion(maximoAcumulado, acumulado)){	
					//Si la solucion que hemos encontrado es mejor que la que teniamos, la guardamos
					maximoAcumulado = acumulado;
					copiarSolucion(solucion, mejorSolucion, n);
				}
			}
			//Si la mesa aun no está llena, llamamos de nuevo con una posición más
			else if(esMejorSolucion( maximoAcumulado, acumulado + max ))
				mesaRedonda( matrizMejores, max, comensales, n, acumulado, solucion,
								actual+1, mejorSolucion, maximoAcumulado, nodos);

			levantaPersona(matrizMejores, max, solucion, acumulado, comensales, actual, i);//desmarcamos el sentado
		}
	}
}

/* Poda con estimación de beneficio  */
