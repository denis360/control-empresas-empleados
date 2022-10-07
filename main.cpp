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
string nombre_empresa;
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

  matriz_usuarios_empresa = vector_matriz(vector_usuarios_empresa, 9);
  matriz_tabla(matriz_usuarios_empresa, 9, vector_usuarios_empresa.size());
}

void agregar_empresa (void) {
  string conf_empresa;
  cout<<"\n\t¿Deseas agregar una nueva emrpesa? [s/n]: "; cin>>conf_empresa;
  if ( conf_empresa == "n" ) return;
  string codigo, nombre, departamento, direccion;
  codigo = to_string(vector_empresas.size()/4).append(timestamp_()).append(to_string(vector_empresas.size()));
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
      nombre_empresa = vector_empresas[c_em+1];
      cout<<"\n\t!Empresa encontrada!"<<endl;
      cout<<"\n\t| ";
      for ( int columna = c_em; columna<c_em+4; columna++ ) {
        cout<<vector_empresas[columna]<<" | ";
      }
      cout<<endl;
      return;
      break;
    }
  }
  cout<<"\n\t!No se encontro la empresa!"<<endl;
  cout<<"\t  1. Inscribe una nueva empresa."<<endl;
  cout<<"\t  2. Verifica tu codigo de empresa."<<endl;
  return agregar_empresa();
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
  string conf_usuario;
  cout<<"\n\t¿Deseas agregar un usuario nuevo? [s/n]: "; cin>>conf_usuario;
  if (conf_usuario == "n") return;
  string codigo, nombre, cargo, sueldo;
  codigo = to_string(vector_usuarios_empresa.size()/9).append(timestamp_()).append(codigo_empresa.substr(2, 4));

  cout<<"\n\tCódigo: "<<codigo<<endl;
  cin.ignore(); cout<<"\n\tNombre: "; getline(cin, nombre);
  cin.ignore(); cout<<"\tCargo: "; getline(cin, cargo);
  cout<<"\n\tStatus: A"<<endl;
  cout<<"\n\tEstado: Alto"<<endl;
  cout<<"\n\tEstado sueldo: Con sueldo"<<endl;
  cin.ignore(); cout<<"\tSueldo: "; getline(cin, sueldo);
  cout<<"\n\tDescripción: Contratado"<<endl;
  cout<<"\n\tEmpresa: "<<codigo_empresa<<endl;

  vector_usuarios.push_back(codigo);
  vector_usuarios.push_back(nombre);
  vector_usuarios.push_back(cargo);
  vector_usuarios.push_back("A");
  vector_usuarios.push_back("Alto");
  vector_usuarios.push_back("Con sueldo");
  vector_usuarios.push_back(sueldo);
  vector_usuarios.push_back("Contratado");
  vector_usuarios.push_back(codigo_empresa);

  guardar_usuarios();

  cout<<"\n\t!Usuario registrado satisfacoriamente!"<<endl;

}

void actualizar_usuario (void) {
  string conf_usuario;
  cout<<"\n\t¿Deseas actualizar un usuario? [s/n]: "; cin>>conf_usuario;
  if (conf_usuario == "n") return;

  string codigo_usuario;
  cout<<"\n\tBuscar código usuario: "; cin>>codigo_usuario;

  int it = 0;
  bool us_val = false;
  for (string elemento : vector_usuarios) {
    if (elemento == codigo_usuario) {
      us_val = !us_val;
      break;
    }
    it++;
  }

  if (!us_val) {
    cout<<"\n\t!No se encontro el usuario!"<<endl;
    return;
  }

  cout<<"\n\tCódigo: "<<vector_usuarios[it]<<endl<<endl;
  cout<<"\tNombre: "<<vector_usuarios[it+1]<<endl;
  cin.ignore(); cout<<"\n\tCargo: "; getline(cin, vector_usuarios[it+2]);
  cin.ignore(); cout<<"\tStatus: "; getline(cin, vector_usuarios[it+3]);

  if (vector_usuarios[it+3] == "S") {
    vector_usuarios[it+4] = "Suspendido";
    vector_usuarios[it+5] = "Sin sueldo";
    vector_usuarios[it+6] = "-";
    cout<<"\n\tSueldo: "<<vector_usuarios[it+6]<<endl;
    vector_usuarios[it+7] = "Suspendido por el igss";
  }

  if (vector_usuarios[it+3] == "N") {
    vector_usuarios[it+4] = "Normal";
    vector_usuarios[it+5] = "Con sueldo";
    vector_usuarios[it+7] = "";
  }

  if (vector_usuarios[it+3] == "B") {
    vector_usuarios[it+4] = "Baja";
    vector_usuarios[it+5] = "Sin sueldo";
    vector_usuarios[it+7] = "Despedido/Renuncia";
  }

  if (vector_usuarios[it+3] != "S") {
    cin.ignore(); cout<<"\tSueldo: "; getline(cin, vector_usuarios[it+6]);
  }

  cout<<"\n\tEmpresa: "<<vector_usuarios[it+8]<<endl;

  guardar_usuarios();

  cout<<"\n\t!Usuario actualizado satisfacoriamente!"<<endl;

}

void generar_reporte (void) {
  ifstream html; html.open("./static/template.html");
  string nombre_reporte = "reporte";
  nombre_reporte.append(timestamp()).append(".html");
  ofstream reporte; reporte.open("./reportes/"+nombre_reporte);

  matriz_usuarios = vector_matriz(vector_usuarios, 9);
  while (getline(html, linea)) {
    reporte << linea << endl;
    if ( linea == "<tbody>" ) {
      for ( int fila = 0; fila<vector_usuarios.size()/9; fila++ ) {
        string sueldo_ = *(*(matriz_usuarios+fila)+6);
        if ( sueldo_ != "-" ) {
          float sueldo = stof(sueldo_);
          if ( sueldo < 2800 | sueldo > 3000 ) {
            reporte << "<tr scope='row'>" << endl;
            for ( int fc = 0; fc<9; fc++ ) {
              reporte << "<td scope='col'>" << *(*(matriz_usuarios+fila)+fc) << "</td>" << endl;
            }
            reporte << "</tr>" << endl;
          }
        }
        if (*(*(matriz_usuarios+fila)+3) == "S") {
          for ( int fc = 0; fc<9; fc++ ) {
            reporte << "<td scope='col'>" << *(*(matriz_usuarios+fila)+fc) << "</td>" << endl;
          }
          reporte << "</tr>" << endl;
        }
      }
    }
  }
  reporte.close();
  html.close();

  cout<<"\n\t!Reporte generado satisfacoriamente!"<<endl;
}

void crear_planilla (void) {
  string nueva_planilla = "./planillas/";

  nueva_planilla.append(codigo_empresa+"_").append(timestamp()+".db");

  string conf_usuarios;
  cout<<"\n\t¿Deseas generar una nueva planilla? [s/n]: "; cin>>conf_usuarios;
  if (conf_usuarios == "n") return;

  ofstream planilla;

  for ( int elemento = 3; elemento<vector_usuarios.size(); elemento+=9 ) {
    if ( vector_usuarios[elemento] == "A" & vector_usuarios[elemento+5] == codigo_empresa ) {
      vector_usuarios[elemento] = "N";
      vector_usuarios[elemento+1] = "Normal";
      vector_usuarios[elemento+4] = "";
    }
  }

  planilla.open(nueva_planilla);
  for ( auto elemento : vector_usuarios ) {
    planilla << elemento << endl;
  }
  planilla.close();

  guardar_usuarios();
}

int main () {

  bool conf = true;

  while ( conf ) {
    obtener_empresas();
    obtener_usuarios();
    abrir_planilla();

    cout<<(codigo_empresa != "" ? "\n\tCódigo: "+codigo_empresa : "");
    cout<<(nombre_empresa != "" ? "\n\tNombre: "+nombre_empresa+"\n" : "");
    cout<<(nombre_planilla != "" ? "\n\tPlanilla: "+nombre_planilla+"\n" : "");

    int conf_opcion = 0;
    cout<<( codigo_empresa == "" ? "\n  1. Buscar empresa" : "\n  1. Mostrar usuarios emrpesa")<<endl;
    cout<<( codigo_empresa == "" ? "  2. Registrar empresa" : "  2. Crear planilla")<<endl;
    cout<<( codigo_empresa == "" ? "  3. Salir" : "  3. Ver planillas")<<endl;
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  4. Mostrar planilla\n" : ""));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  5. Registrar usuario\n" : "  4. Registrar usuario\n"));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  6. Actualizar usuario\n" : "  5. Actualizar usuario\n" ));
    cout<<( codigo_empresa == "" ? "" : ( nombre_planilla != "" ? "  7. Generar reporte\n" : "" ));
    cout<<"Seleccione (1-): "; cin>>conf_opcion;

    system("clear");
    if ( conf_opcion == 1 & codigo_empresa == "") buscar_empresa();
    if ( conf_opcion == 1 & codigo_empresa != "") mostrar_matriz(matriz_usuarios_empresa, 9, vector_usuarios_empresa.size());
    if ( conf_opcion == 2 & codigo_empresa == "") agregar_empresa ();
    if ( conf_opcion == 2 & codigo_empresa != "") crear_planilla();
    if ( conf_opcion == 3 & codigo_empresa != "") mostrar_planillas();
    if ( conf_opcion == 4 & codigo_empresa != "") mostrar_matriz(matriz_planilla, 9, vector_planilla.size());

    if ((conf_opcion == 5 & codigo_empresa != "" & nombre_planilla != "") |
        (conf_opcion == 4 & nombre_planilla == ""))
      agregar_usuario();

    if ((conf_opcion == 6 & codigo_empresa != "" & nombre_planilla != "") |
        (conf_opcion == 5 & nombre_planilla == ""))
      actualizar_usuario();

    if ( conf_opcion == 7 & codigo_empresa != "") generar_reporte();

  }

  return 0;
}

