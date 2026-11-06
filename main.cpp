#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <windows.h>

#include "ControlParques.h"
#include "Arbol.h"
using namespace std;

//Rutas de los archivos
const string RUTA_ESPECIES = "arbolado-especies.csv";
const string RUTA_PARQUES = "parques.csv";
const string RUTA_ARBOLES = "arboladoMadrid.csv";

void mostrarInfoArbol(Arbol* arbol) {
    cout << "\t" << "identificador: " << arbol->getAssetnum() << endl;
    cout << "\t" << "codigo de parque: " << arbol->getParque()->get_codigo_parque() << endl;
    cout << "\t" << "nombre común: " << arbol->getEspecie()->getnombre_comun() << endl;
    cout << "\t" << "nombre científico: " << arbol->getEspecie()->getnombre_cientifico() << endl;
    cout << "\t" << "tipo planta: " << arbol->getEspecie()->gettipo_planta() << endl;
    cout << endl;
}

void mostrarInfoParque(Parque* pq) {
    cout << "\tCodigo: " << pq->get_codigo_parque() << endl;
    cout << "\tNombre: " << pq->get_nombre_parque() << endl;
    cout << "\tArboles: " << pq->getArboles().size() << endl;
    cout << "\tEspecies: " << pq->contarEspecies() << endl;
    cout << "\tTipos: " << pq->contarTipos() << endl;
    cout << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    try {
        ControlParques control(
            RUTA_ESPECIES,
            RUTA_PARQUES,
            RUTA_ARBOLES);

        std::cout << "\nEJERCICIO 1: BUSCAR VARIOS ID\n";
        string identificadores[] = {"6951831", "10152826", "8887777", "6906686"};
        for (string id: identificadores) {
            auto arbol = control.buscarArbol(id);
            if (arbol == nullptr) {
                cout << "\t" << id << " no encontrado" << endl << endl;
                continue;
            }
            mostrarInfoArbol(arbol);
        }

        cout << "\nEJERCICIO 2: ESPECIE MÁS COMÚN EN PARQUE PRINCESA LEONOR\n";
        Especie *especieMasComun = control.buscarParque("PARQUE PRINCESA LEONOR")->especieMasComun();
        if (especieMasComun == nullptr) {
            cout << "\t" << "No se ha encontrado el parque" << endl;
        } else {
            cout << "\t" << "Especie más común: " << especieMasComun->getnombre_cientifico() << endl;
        }

        cout << "\nEJERCICIO 3: BUSCAR PARQUES CON ABETO\n";
        auto parques = control.listarParquesArbol("Abeto");
        for (Parque *pq: parques) {
            mostrarInfoParque(pq);
        }

        cout << "\nEJERCICIO 4: BUSCAR BUSCAR PINO RESINERO Y PINO\n";
        string unicoArbol = "Pino resinero";
        std::vector<Arbol*> arbol = control.buscarArbolNcomun(unicoArbol);
        if (arbol.empty()) {
            cout << "\t" << "No se ha encontrado el árbol " << unicoArbol << endl;
        } else {
            cout << "\tEjemplares de " << unicoArbol << ": " << arbol.size() << endl;
            for (unsigned int i = 0; i < arbol.size() && i < 5; i++) {
                Arbol* arbool = arbol[i];
                mostrarInfoArbol(arbool);
            }
        }

        vector<Arbol *> arbolesPino = control.buscarArbolNcomunComienzo("Pino");
        cout<<"\tPinos encontrado: "<<arbolesPino.size()<<endl;

        cout << "\nEJERCICIO 5: ÁRBOLES EN BARRIOS 43 Y 91\n";
        int id_barrios[] = {43, 91};
        for (int barrio: id_barrios) {
            int total = control.contarArbolesBarrio(barrio);
            cout << "\t" << "Total árboles barrio " << barrio << ": " << total << endl;
        }
        cout<<"======================================================================"<<endl;
        cout<<"EJERCICIO POR PAREJAS:"<<endl;
        vector<Arbol*> pD9= control.buscarArbolPorEspecieYDistrito("Pino",9);
        cout<<"Encontrados: "<<pD9.size()<<endl;
        for (unsigned int i = 0; i < pD9.size(); i++) {
            if (pD9[i]!= nullptr) {
                cout<<"Parque: "<<pD9[i]->getParque()->get_nombre_parque()<<endl;
            }else {
                cout<<"No esta en ningun parque";
            }
            mostrarInfoArbol(pD9[i]);

        }

    }catch (const std::invalid_argument& e) {
        cout << e.what() << endl;
    }catch (const std::out_of_range& e) {
        cout << e.what() << endl;
    }catch (const std::runtime_error& e) {
        cout << e.what() << endl;
    } catch (const std::exception &error) {
        std::cerr << "\nError: " << error.what() << '\n';
        return 1;
    }

    return 0;
};
