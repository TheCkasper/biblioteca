#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;
const int maxilibros = 100;

struct Libro {
    int codigo;
    string titulo;
    string autor;
    int anio;
    string estado; // "disponible" o "prestado"
};

void gotoxy(int x, int y){
COORD coordinate;
coordinate.X = x;
coordinate.Y = y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}
void cambiarColorTexto(int color) {
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hConsole, color);
}

// VARIABLE GLOBAL
Libro biblioteca[maxilibros];
int tolibros = 0;

void menu() {
	system("cls");
	cambiarColorTexto(6);
	gotoxy(70,11);cout<<"ALERT: Recuerda Cargar la informacion antes de realizar una operacion. (7)";
	gotoxy(70,13);cout<<"ALERT: Recuerda Guardar la informacion despues de realizar una operacion. (6)";
	cambiarColorTexto(8);
    gotoxy(80,16);cout<<"========== MENU PRINCIPAL ==========";
    gotoxy(80,17);cout<<"1. Registrar libro";
    gotoxy(80,18);cout<<"2. Mostrar listado de libros";
    gotoxy(80,19);cout<<"3. Buscar un libro";
    gotoxy(80,20);cout<<"4. Prestar un libro";
    gotoxy(80,21);cout<<"5. Devolver un libro";
    gotoxy(80,22);cout<<"6. Guardar informacion en archivo";
    gotoxy(80,23);cout<<"7. Cargar informacion desde archivo";
    gotoxy(80,24);cout<<"8. Salir";
}

int buscarcodigo(int codigo) {
    for(int i=0;i<tolibros;i++) {
        if(biblioteca[i].codigo==codigo) {
            return i;
        }
    }
    return -1;
}
bool duplicado(int codigo) {
    return buscarcodigo(codigo) != -1;
}

//PARA REGISTRAR
void reglibro(){
    if(tolibros>=maxilibros) {
    	system("cls");
    	cambiarColorTexto(4);
        gotoxy(80,16);cout<<"ERROR: Ya son demasiados libros registrados bro.";
        system("pause>nul");
        cambiarColorTexto(8);
        return;
    }
    
    system("cls");
    Libro nuevolibro;
    cambiarColorTexto(9);
    gotoxy(80,16);cout<<"========== REGISTRAR LIBRO ==========";
    
    //  ciclo para evitar duplicados
    do {
        gotoxy(80,17);cout<<"Codigo del libro: ";
        cin>>nuevolibro.codigo;
        cin.ignore();
        
        if (duplicado(nuevolibro.codigo)) {
        	system("cls");
        	cambiarColorTexto(4);
            gotoxy(80,16);cout << "ERROR: Este codigo ya existe. Intente con otro.";
            system("pause>nul");
            cambiarColorTexto(9);
        }
    } while (duplicado(nuevolibro.codigo));
    
    gotoxy(80,18);cout<<"Titulo: ";
    getline(cin, nuevolibro.titulo);
    
    gotoxy(80,19);cout<<"Autor: ";
    getline(cin, nuevolibro.autor);
    
    gotoxy(80,20);cout<<"Anio de publicacion: ";
    cin>>nuevolibro.anio;
    
    // Por defecto, el libro está disponible
    nuevolibro.estado = "disponible";
    
    // Agregar el libro al arreglo
    biblioteca[tolibros] = nuevolibro;
    tolibros++;
    cambiarColorTexto(10);
    gotoxy(80,22);cout<<"LIBRO REGISTRADO!";
    system("pause>nul");
}
//PARA GUARDAR
void guardarlibro() {
    ofstream archivo("libros.txt");
    
    if (!archivo.is_open()){
    	system("cls");
    	cambiarColorTexto(4);
    	gotoxy(80,15);cout<<"ERROR: No se pudo abrir el archivo para guardar.";
    	system("pause>nul");
        return;
    }
    
    // Guardar
    for (int i=0;i<tolibros;i++) {
        archivo<<biblioteca[i].codigo<<"|"<<biblioteca[i].titulo<<"|"<<biblioteca[i].autor<<"|"<<biblioteca[i].anio<<"|"<<biblioteca[i].estado<<endl;
    }
    archivo.close();
    system("cls");
    cambiarColorTexto(10);
    gotoxy(80,16);cout<<"Informacion guardada exitosamente en 'libros.txt'.";
    system("pause>nul");
}
//CUENTA LIBROS DE libros.txt
void contarlibros(int &disponible, int &prestado) {
    disponible = 0;
    prestado = 0;
    
    for (int i=0;i<tolibros;i++){
        if (biblioteca[i].estado== "disponible") {
            disponible++;
        } else {
            prestado++;
        }
    }
}
//PARA VER LOS REGISTROS
void verlibro() {
	system("cls");
    int ejeY=5;
    cambiarColorTexto(9);
    gotoxy(80,ejeY++);cout<<"========== LISTADO DE LIBROS ==========";
    
    if (tolibros == 0) {
    	cambiarColorTexto(4);
        gotoxy(80,ejeY++);cout<<"No hay libros registrados.";
        system("pause>nul");
        return;
    }
    
    for (int i=0;i<tolibros;i++) {
    	cambiarColorTexto(10);
        gotoxy(80,ejeY++);cout<<"LIBRO #"<<(i + 1);
        gotoxy(80,ejeY++);cout<<"Codigo: "<<biblioteca[i].codigo;
        gotoxy(80,ejeY++);cout<<"Titulo: "<<biblioteca[i].titulo;
        gotoxy(80,ejeY++);cout<<"Autor: "<<biblioteca[i].autor;
        gotoxy(80,ejeY++);cout<<"Anio: "<<biblioteca[i].anio;
        gotoxy(80,ejeY++);cout<<"Estado: "<<biblioteca[i].estado;
        gotoxy(80,ejeY++);cout<<"-----------------------";
        
    }
    
    int disponible, prestado;
    contarlibros(disponible, prestado);
    
    gotoxy(80,ejeY++);cout<<"Total de libros: "<< tolibros;
    gotoxy(80,ejeY++);cout<<"Disponibles: "<<disponible<<" | Prestados: "<<prestado;
    system("pause>nul");
}
//PARA VER LOS DATOS GUARDADOS ANTERIORMENTE
void cargardatos() {
    ifstream archivo("libros.txt");
    
    if (!archivo.is_open()) {
    	system("cls");
    	cambiarColorTexto(14);
        gotoxy(80,13);cout << "Biblioteca vacia, empieza a registrar.";
        system("pause>nul");
        return;
    }
    
    tolibros = 0;
    string linea;
    
    while (getline(archivo, linea) && tolibros<maxilibros) {
        Libro libro;
        size_t pos = 0;
        string datos[5];
        int campo=0;
        
        for (size_t i=0; i<linea.length();i++) {
            if (linea[i]=='|') {
                campo++;
            } else {
                datos[campo]+=linea[i];
            }
        }
        
        libro.codigo = stoi(datos[0]);
        libro.titulo = datos[1];
        libro.autor = datos[2];
        libro.anio= stoi(datos[3]);
        libro.estado = datos[4];
        
        biblioteca[tolibros] = libro;
        tolibros++;
    }
    
    archivo.close();
    system("cls");
    cambiarColorTexto(10);
    gotoxy(80,14);cout<<"Se cargaron "<<tolibros<<" libros guardados.";
    system("pause>nul");
}
//PARA BUSCAR POR CODIGO
int buscodigo(int codigo) {
    for (int i=0;i< tolibros;i++) {
        if (biblioteca[i].codigo==codigo) {
            return i; // Retorna
        }
    }
    return -1; // Retorna -1 quiere decir que no se encontro
}
//PARA BUSCAR POR EL TITULO
int bustitulo(string titulo) {
    for (int i=0;i<tolibros;i++) {
        // Comparar si el título esta
        if (biblioteca[i].titulo.find(titulo) != string::npos) { //npos, constante de string sin posicion
            return i;
        }
    }
    return -1;
}
//PARA BUSCAR LIBROS REGISTRADOS
void buslibro() {
    int opbuscar;
    system("cls");
    cambiarColorTexto(9);
    gotoxy(80,15);cout<<"========== BUSCAR LIBRO ==========";
    gotoxy(80,16);cout<<"1. Buscar por codigo";
    gotoxy(80,17);cout<<"2. Buscar por titulo";
    gotoxy(80,18);cout<<"SELECCIONA QUE DESEA BUSCAR: ";
    cin >> opbuscar;
    cin.ignore();
    
    int encontro = -1; // -1 si no se encontro el libro
    
    if (opbuscar==1) {
        // Buscar por código
        int codigo;
        system("cls");
        gotoxy(80,16);cout<<"Ingrese el codigo: ";
        cin>>codigo;
        encontro = buscodigo(codigo);
    } 
    else if (opbuscar==2) {
        // Buscar por titulo
        string titulo;
        system("cls");
        gotoxy(80,16);cout<<"Ingrese el titulo: ";
        getline(cin,titulo);
        encontro = bustitulo(titulo);
    }
    else {
    	system("cls");
    	cambiarColorTexto(4);
        gotoxy(80,16);cout << "Opcion invalida.";
        system("pause>nul");
        return;
    }
    
    // Mostrar resultado
    if (encontro == -1) {
    	system("cls");
    	cambiarColorTexto(4);
        gotoxy(80,15);cout << "No encontrado.";
        system("pause>nul");
    } else {
    	system("cls");
    	cambiarColorTexto(10);
        gotoxy(80,15);cout<<"--- LIBRO ENCONTRADO ---";
        gotoxy(80,16);cout<<"Codigo: "<<biblioteca[encontro].codigo;
        gotoxy(80,17);cout<<"Titulo: "<<biblioteca[encontro].titulo;
        gotoxy(80,18);cout<<"Autor: "<<biblioteca[encontro].autor;
        gotoxy(80,19);cout<<"Anio: "<<biblioteca[encontro].anio;
        gotoxy(80,20);cout<<"Estado: "<<biblioteca[encontro].estado;
        gotoxy(80,21);cout<<" ";
        system("pause>nul");
    }
}
//CUANDO SE PRESTA UN LIBRO
void prestalibro() {
    int codigo;
    
    system("cls");
    cambiarColorTexto(9);
    gotoxy(80,15);cout<<"========== PRESTAR LIBRO ==========";
    gotoxy(80,16);cout<<"Ingrese el codigo del libro: ";
    cin>>codigo;
    
    int encontrar = buscodigo(codigo);
    
    if (encontrar == -1) {
    	cambiarColorTexto(4);
        gotoxy(80,18);cout<<"ERROR: Libro no encontrado.";
        system("pause>nul");
        return;
    }
    
    if (biblioteca[encontrar].estado == "prestado") {
    	cambiarColorTexto(4);
        gotoxy(80,18);cout<<"ERROR: Este libro ya esta prestado.";
        system("pause>nul");
        return;
    }
    
    // Cambiar estado a prestado
    biblioteca[encontrar].estado = "prestado";
    cambiarColorTexto(10);
    gotoxy(80,18);cout<<"Libro '"<<biblioteca[encontrar].titulo<<"' prestado exitosamente.";
    system("pause>nul");
}
//DEVOLVER EL LIBRO
void devlibro() {
    int codigo;
    system("cls");
    cambiarColorTexto(9);
    gotoxy(80,15);cout<<"========== DEVOLVER LIBRO ==========";
    gotoxy(80,16);cout<<"Ingrese el codigo del libro: ";
    cin>>codigo;
    
    int encontrar = buscodigo(codigo);
    
    if (encontrar==-1) {
    	cambiarColorTexto(4);
        gotoxy(80,18);cout<<"ERROR: Libro no encontrado.";
        system("pause>nul");
        return;
    }
    
    if (biblioteca[encontrar].estado == "disponible") {
    	cambiarColorTexto(14);
        gotoxy(80,18);cout<<"ADVERTENCIA: Este libro ya estaba disponible.";
        system("pause>nul");
        return;
    }
    
    // Cambiar estado a disponible
    biblioteca[encontrar].estado = "disponible";
    cambiarColorTexto(10);
    gotoxy(80,18);cout<<"Libro '"<<biblioteca[encontrar].titulo<<"' devuelto exitosamente.";
    system("pause>nul");
}

int main() {
	int op;
	bool activo=true;
    
    while(activo){
    	menu();
    	gotoxy(80,25);cout<<"SELECCIONE UNA OPCION: ";
    	cin>>op;
    	
    	switch (op){
    		case 1:
    			reglibro();
    			break;
    		case 2:
    			verlibro();
    			break;
    		case 3:
    			buslibro();
    			break;
    		case 4:
    			prestalibro();
    			break;
    		case 5:
    			devlibro();
    			break;
    		case 6:
    			guardarlibro();
    			break;
    		case 7:
    			cargardatos();
    			break;
    		case 8:
    			system("cls");
    			gotoxy(80,16);cout<<"Guardando Cambios...";
    			guardarlibro();
    			gotoxy(80,17);cout<<"Has salido del sistema...";
    			activo=false;
    			break;
    		default:
    			system("cls");
    			cambiarColorTexto(4);
    			gotoxy(80,15);cout<<" ERROR: OPCION INVALIDA. INTENTE NEUVAMENTE...";
    			system("pause>nul");
		}
	}
    
	return 0;
}
