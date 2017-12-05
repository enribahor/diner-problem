#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int N = 12;
int matrizAfinidad[N][N];

void mesaRedonda(bool comensales[], int n, int acumulado, int solucion[], int actual, int mejorSolucion[], int &maximoAcumulado, int &nodos, int afinidadMaxima);
void mesaRedonda( int n, int solucionFinal[], int &acumuladoFinal, int &nodosVisitados);
void cargarMatriz(string nombreArchivo);

int main(){
	int  mejorSolucion[N], maximoAcumulado = 0, nodos = 0;
	cargarMatriz("Datos1.txt");
	int time0 = clock();
	mesaRedonda( N,  mejorSolucion, maximoAcumulado, nodos);
	
	cout << "	Numero de comensales: " << N << endl;
	cout << "	Tiempo: " << (clock() - time0) << " milisegundos" << endl;
	cout << endl << "	Maximo acumulado: " << maximoAcumulado;
	cout << "	Nodos visitados: " << nodos << endl;
	
	ofstream fOut;
	fOut.open("Resultado1.txt", ios_base::app);
	fOut << endl << "	Poda: Maxima afinidad total por numero de afinidades que faltan por colocar" << endl;
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
	if(b!=a) return b*a;
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
void mesaRedonda( int n, int solucionFinal[], int &acumuladoFinal, int &nodosVisitados){
	bool *comensales = new bool[n];
	for(int i = 0; i < n; i++)
		comensales[i] = true;
	int *solucion = new int[n];
	solucion[0] = 0;
	comensales[0] = false;

	/* Calculamos la afinidad máxima en la tabla*/
	int afinidadMaxima = 0;
	for(int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if( afinidad(i,j) > afinidadMaxima) afinidadMaxima = afinidad(i,j);
		}
	}
	
	

	/* Llamada inicial:
		-comensales: Array que indica los comensales que no han sido colocados. Comienza con el 
	primer comensal ya colocado.
		-n: Número de comensales.
		-acumulado: valor acumulado en la llamada actual, comienza en cero.
		-solucion[]: contiene la posicion de los comensales que se está probando,
	hasta la posicion actual. -Comienza con el comensal 0 fijado.
		-actual: indica la posicion de la mesa en la que se está colocando
	al comensal: comienza en la posicion 1, pues la 0 ya esta determinada.
		-solucionFinal: array donde se guardará la mejor solucion.
		-acumuladoFinal y nodosVisitados, donde se guardarán ambos datos */
	
	mesaRedonda(comensales, n, 0, solucion, 1, solucionFinal, acumuladoFinal, nodosVisitados, afinidadMaxima);
}
/* Debes comenzar con un comensal fijado en la posicion 0 */
void mesaRedonda(bool comensales[], int n, int acumulado, int solucion[], int actual, int mejorSolucion[], int &maximoAcumulado, int &nodos, int afinidadMaxima){
	nodos++;//Actualizamos los nodos visitados
	for(int i = 1; i < n; i++){
		if( comensales[i] )	/*Si el comensal ya está en la mesa sentado, se pasa al siguiente*/{		
			solucion[actual] = i;		//Colocamos al comensal 'i' en la posicion de la mesa que toca (actual)
			comensales[i]  = false;		//Marcamos al comensal como sentado
			// Actualizamos la afinidad con la persona que tiene a la izquierda (inicialmente debe haber ya una sentada)
			acumulado += afinidad(solucion[actual-1], solucion[actual]);	
			if(esSolucion(solucion,actual, n)){		//Si la mesa ya está llena, tratamos la solucion
				// Para el resultado final actualizamos la afinidad del último con el primero
				acumulado += afinidad( solucion[actual], solucion[0]); 
				if(maximoAcumulado < acumulado){	
					//Si la solucion que hemos encontrado es mejor que la que teniamos, la guardamos
					maximoAcumulado = acumulado;
					copiarSolucion(solucion, mejorSolucion, n);
				}
			}
			//Si la mesa aun no está llena, llamamos de nuevo con una posición más
			else if( (acumulado + (afinidadMaxima*(n-actual))) > maximoAcumulado) mesaRedonda(comensales, n, acumulado, solucion, actual+1, mejorSolucion, maximoAcumulado, nodos, afinidadMaxima);
			//Desmarcamos los comensales y el valor acumulado;
			acumulado -= afinidad(solucion[actual-1], solucion[actual]);
			comensales[i] = true;
		}
	}
}
/*Cota 1: Afinidad máxima*Afinidades que quedan por sumar. */
