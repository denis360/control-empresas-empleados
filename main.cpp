#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std;

vector<string>vector_empresas;
vector<string>vector_usuarios;
vector<string>vector_planilla;
vector<string>vector_usuarios_empresa;

string** matriz_empresas;
string** matriz_usuarios;
string** matriz_planilla;
string** matriz_usuarios_empresa;

string nombre_planilla;
string codigo_empresa;
string linea;

stringstream ss;
stringstream stime;

#include "funciones.h"

string timestamp_ () {
  stime.str("");
  time_t end_time = time(nullptr);
  tm* now = localtime(&end_time);
  char buffer[128];
  strftime(buffer, sizeof(buffer), "-%y%d-", now);
  stime << buffer;
  return stime.str();
}

string timestamp () {
  stime.str("");
  time_t end_time = time(nullptr);
  tm* now = localtime(&end_time);
  char buffer[128];
  strftime(buffer, sizeof(buffer), "_%d-%m-%Y_%H:%M:%S", now);
  stime << buffer;
  return stime.str();
}

void guardar_empresas (void) {
  ofstream empresas_; empresas_.open("./bases-datos/empresas.db");
  for ( string elemento : vector_empresas ) {
    empresas_ << elemento << endl;
  }
  empresas_.close();
}

void guardar_usuarios (void) {
  ofstream usuarios_; usuarios_.open("./bases-datos/usuarios.db");
  for ( string elemento : vector_usuarios ) {
    usuarios_ << elemento << endl;
  }
  usuarios_.close();
}

void obtener_empresas (void) {
  ifstream empresas; empresas.open("./bases-datos/empresas.db");
  if (!empresas.is_open()) return;
  vector_empresas.clear();
  while (getline(empresas, linea)) {
    vector_empresas.push_back(linea);
  };
  empresas.close();
  matriz_empresas = vector_matriz(vector_empresas, 9);
}

void obtener_usuarios (void) {
  ifstream usuarios; usuarios.open("./bases-datos/usuarios.db");
  if (!usuarios.is_open()) return;
  vector_usuarios.clear();

  while (getline(usuarios, linea)) vector_usuarios.push_back(linea);
  usuarios.close();

  int it = 0;
  vector_usuarios_empresa.clear();
  for ( string elemento : vector_usuarios ) {
    if ( codigo_empresa != "" & codigo_empresa == elemento ) {
      for ( int c_us = it-8; c_us<=it; c_us++ ) {
        vector_usuarios_empresa.push_back(vector_usuarios[c_us]);
      }
    }
    it++;
  }

  matriz_usuarios = vector_matriz(vector_usuarios_empresa, 9);
  matriz_tabla(matriz_usuarios, 9, vector_usuarios_empresa.size());
}

void agregar_empresa (void) {
  string conf_empresa;
  cout<<"\n\t¿Deseas agregar una nueva emrpesa? [s/n]: "; cin>>conf_empresa;
  if ( conf_empresa == "n" ) return;
  string codigo, nombre, departamento, direccion;
  codigo = to_string(vector_empresas.size()/4).append(timestamp_()).append("em");
  cout<<"\n\tCódigo: "<<codigo<<endl<<endl;
  cin.ignore(); cout<<"\tNombre empresa: "; getline(cin, nombre);
  cin.ignore(); cout<<"\tDepartamento: "; getline(cin, departamento);
  cin.ignore(); cout<<"\tDirección: "; getline(cin, direccion);

  vector_empresas.push_back(codigo);
  vector_empresas.push_back(nombre);
  vector_empresas.push_back(departamento);
  vector_empresas.push_back(direccion);

  guardar_empresas();

  cout<<"\n\t!Empresa registrada satisfacoriamente"<<endl;
}

void buscar_empresa (void) {
  string codigo;
  cout<<"Ingrese codigo de empresa: "; cin>>codigo;
  for ( int c_em = 0; c_em<vector_empresas.size(); c_em+=4 ) {
    if ( vector_empresas[c_em] == codigo ) {
      codigo_empresa = codigo;
      cout<<"\n\t!Empresa encontrada!"<<endl;
      cout<<"\n\t| ";
      for ( int columna = c_em; columna<c_em+4; columna++ ) {
        cout<<vector_empresas[columna]<<" | ";
      }
      cout<<endl;
      break;
    }
  }
}

void abrir_planilla (void) {
  ifstream planilla_archivo; planilla_archivo.open("./planillas/"+nombre_planilla);
  if (nombre_planilla == "") return;
  if (!planilla_archivo.is_open()) return;

  vector_planilla.clear();
  while (getline(planilla_archivo, linea)) vector_planilla.push_back(linea);
  planilla_archivo.close();

  matriz_planilla = vector_matriz(vector_planilla, 9);
  matriz_tabla(matriz_planilla, 9, vector_planilla.size());
}

void mostrar_planillas (void) {
  string path = "./planillas/";
  int it = 0;
  string nombre;
  string codigo_planilla;
  for ( const auto &elemento : filesystem::directory_iterator(path) ) {
    string nombre_planilla;
    ss.str("");
    ss << elemento;
    nombre_planilla = ss.str();
    nombre_planilla.erase(remove(nombre_planilla.begin(), nombre_planilla.end(), '\"'), nombre_planilla.end());
    nombre = nombre_planilla.substr(path.length(), nombre_planilla.length());
    codigo_planilla = nombre.substr(0, codigo_empresa.length());
    if ( codigo_planilla == codigo_empresa ) {
      cout<<"["<<it+1<<"] -> "<<nombre<<endl;
      it++;
    }
  }
  int numero_planilla = 0;
  it = 0;
  cout<<"Seleccione (1-): "; cin>>numero_planilla;
  for ( const auto &elemento : filesystem::directory_iterator(path) ) {
    string n_planilla;
    ss.str("");
    ss << elemento;
    n_planilla = ss.str();
    n_planilla.erase(remove(n_planilla.begin(), n_planilla.end(), '\"'), n_planilla.end());
    nombre = n_planilla.substr(path.length(), n_planilla.length());
    codigo_planilla = nombre.substr(0, codigo_empresa.length());
    if ( codigo_planilla == codigo_empresa ) {
      if ( numero_planilla-1 == it ) {
        nombre_planilla = nombre;
        break;
      }
      it++;
    }
  }
}

void agregar_usuario (void) {

}

void actualizar_usuario (void) {

}

void generar_reporte (void) {

}

int main () {

  bool conf = true;

  while ( conf ) {
    obtener_empresas();
    obtener_usuarios();
    abrir_planilla();

    int conf_opcion = 0;
    cout<<( codigo_empresa == "" ? "\n  1. Buscar empresa" : "\n  1. Mostrar usuarios emrpesa")<<endl;
    cout<<( codigo_empresa == "" ? "  2. Registrar empresa" : "  2. Crear planilla")<<endl;
    cout<<( codigo_empresa == "" ? "  3. Salir" : "  3. Ver planillas")<<endl;
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  4. Mostrar planilla\n" : ""));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  5. Registrar usuario\n" : "  4. Registrar usuario\n"));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  6. Actualizar usuario\n" : "  5. Actualizar usuario\n" ));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  7. Generar reporte\n" : "" ));
    cout<<"Seleccione (1-): "; cin>>conf_opcion;

    if ( conf_opcion == 1 & codigo_empresa == "") buscar_empresa();
    if ( conf_opcion == 1 & codigo_empresa != "") mostrar_matriz(matriz_usuarios, 9, vector_usuarios_empresa.size());
    if ( conf_opcion == 2 & codigo_empresa == "") agregar_empresa ();
    if ( conf_opcion == 2 & codigo_empresa != "") cout<<"Generando planilla"<<endl;
    if ( conf_opcion == 3 & codigo_empresa != "") mostrar_planillas();
    if ( conf_opcion == 4 & codigo_empresa != "") mostrar_matriz(matriz_planilla, 9, vector_planilla.size());
    if ( conf_opcion == 5 & codigo_empresa != "") agregar_usuario();
    if ( conf_opcion == 6 & codigo_empresa != "") actualizar_usuario();
    if ( conf_opcion == 7 & codigo_empresa != "") generar_reporte();

  }

  return 0;
}

