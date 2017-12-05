#include <iostream>
#include <fstream>

using namespace std;

const int N = 17;
const int RANGO_MAX = 200;
int mDatos[N][N];

void generarDatos( string datos  ){
	ofstream fOut;
	fOut.open(datos);
	for ( int i = 0; i < N; i++ ){
		for ( int j = 0; j <=i; j++ ){
			if (i==j) fOut << "		" << '0';
			else fOut <<  "		" << (rand()%RANGO_MAX)+1;
		}
		fOut << endl;
	}
	fOut.close();
}
void cargarDatos(string datos){
	ifstream fIn;
	fIn.open(datos);
	for ( int i = 0; i < N; i++ ){
		for ( int j = 0; j <=i; j++ ){
			if (i==j) fIn >> mDatos[i][j];
			else{
				fIn >> mDatos[i][j];
				mDatos[j][i] = mDatos[i][j];
			}
		}
	}
	fIn.close();
}

void datosDefinitivos(string datos){
	ofstream fOut;
	fOut.open(datos);
	for ( int i = 0; i < N; i++ ){
		for ( int j = 0; j < N; j++ ){
			fOut <<  "		" << mDatos[i][j];
		}
		fOut << endl;
	}
	fOut.close();
}
int main(){
string datos = "Datos1.txt";
generarDatos(datos);
cargarDatos(datos);
datosDefinitivos(datos);
}