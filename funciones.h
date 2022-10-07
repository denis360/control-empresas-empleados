int filas = 0;

int longitud_puntero (string *p) {
  return (*p).length();
}

void mostrar_matriz ( string** matriz, int columnas, int tamaño ) {
  filas = tamaño/columnas;
  for ( int fila = 0; fila<filas; fila++ ) {
    cout<<"| ";
    for ( int columna = 0; columna<columnas; columna++ ) {
      cout<<*(*(matriz+fila)+columna)<<" | ";
    }
    cout<<endl;
  }
}

void matriz_tabla ( string** matriz, int columnas, int tamaño ) {
  int mayor = 0;
  filas = tamaño/columnas;
  for ( int columna = 0; columna<columnas; columna++ ) {
    mayor = 0;
    for ( int fila = 0; fila<filas; fila++ ) {
      int longitud = longitud_puntero(&(*(*(matriz+fila)+columna)));
      mayor = ( mayor < longitud ? longitud : mayor);
    }
    for ( int fila = 0; fila<filas; fila++ ) {
      int longitud_ = longitud_puntero(&(*(*(matriz+fila)+columna)));
      int total_espacios = mayor-longitud_;
      for ( int espacio = 0; espacio<total_espacios; espacio++ ) {
        *(*(matriz+fila)+columna) += " ";
      }
    }
  }
}

/* Recibe un vector y el total de columas que tendra la matriz */
string** vector_matriz ( vector<string>arr, int columnas ) {
  string** matriz;
  filas = arr.size()/columnas; // sacamos el total de filas restando el tamaño del vector con el total de columnas
  matriz = new string*[filas]; // creamos las filas de la matriz
  for ( int fila = 0; fila<filas; fila++ ) {
    matriz[fila] = new string[columnas]; // a cada fina le creamos el total de columnas que se le paso a la funcion
  }
  int it = 0;
  for ( int fila = 0; fila<filas; fila++ ) {
    for ( int columna = 0; columna<columnas; columna++ ) {
      *(*(matriz+fila)+columna) = arr[it++]; // llenamos la matriz con los datos del vector que se le paso a la funcion
    }
  }

  return matriz; // retornamos la matriz con datos
}
