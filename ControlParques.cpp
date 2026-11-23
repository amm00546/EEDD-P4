//
// Created by a_amo on 07/10/2026.
//

#include "ControlParques.h"
#include "LectorCSV.h"
#include <stdexcept>
#include <algorithm>

ControlParques::ControlParques(const std::string& rutaEspecies,
    const std::string& rutaParques, const std::string& rutaArboles):
    trees(134438,0.65, 2){
    LectorCSV lector;

    if (!lector.cargar(especies, rutaEspecies)) {
        throw std::runtime_error("ControlParques: No se pudo abrir "+ rutaEspecies);
    }

    if(!lector.cargarParques(parques, rutaParques)) {
        throw std::runtime_error("ControlParques: No se pudo abrir "+ rutaParques);
    }

    bool ok = lector.cargarArboles(rutaArboles, [this](const Arbol& leido, const std::string& codEspecie) {
        unsigned long clave= djb2(leido.getAssetnum().c_str());
        if (!trees.insertar(clave,leido)) {
            return;
        }
        Arbol* a = trees.buscar(clave);    // puntero al arbol DENTRO del map
        idArboles.push_back(a->getAssetnum());
        a->setEspecie(buscarEspecieCodigo(codEspecie));

        Parque* p = buscarParqueCodigo(a->getNumParque());
        if (p != nullptr) {
            p->insertarArbol(a);            // Parque::contains
            a->setParque(p);                // Arbol::isInto
        }

        if (a->getEspecie() != nullptr) {
            treeName.insert(std::make_pair(a->getNbreComun(), a));
        }
    });

    if (!ok) {
        throw std::runtime_error("ControlParques: no se pudo abrir " + rutaArboles);
    }
}

std::map<std::string,Especie>& ControlParques::get_especies() {
    return this->especies;
}

std::map<int,Parque>& ControlParques::get_parques() {
    return this->parques;
}

int ControlParques::contarEspecieParque(const std::string& nComun) {
    if (nComun.empty()) {
        throw std::invalid_argument("Nombre comun vacio");

    }
    return listarParquesArbol(nComun).size();
}

void ControlParques::asignarEspecieParque(Especie* e, Parque& p) {
    if (e==nullptr) {
        return;
    }
    std::vector<Arbol*> arbolesParque = p.getArboles();
    for (int i = 0; i < arbolesParque.size(); i++) {
        if (arbolesParque[i] != nullptr && arbolesParque[i]->getTipoEspecie() == e->getcodigoEspecie()) {
            arbolesParque[i]->setEspecie(e);
        }
    }
}


std::map<std::string,Especie*> ControlParques::listadoEspeciesSubNbre(const std::string& cadena) {
    std::map<std::string,Especie*> listado;
    for (std::map<std::string, Especie>::iterator it= especies.begin(); it != especies.end(); ++it) {
        const std::string& nombreCientifico = it->second.getnombre_cientifico();
        if (nombreCientifico.find(cadena)!= std::string::npos) {
            listado.insert(std::make_pair(it->first,&(it->second)));
        }
    }
    return listado;
}





Parque* ControlParques::buscarParque(const std::string& nombreParque) {
    for (std::map<int, Parque>::iterator it = parques.begin(); it != parques.end(); ++it) {
        if (it->second.get_nombre_parque() == nombreParque) {
            return &(it->second);
        }
    }
    return nullptr;
}




//METODOS PRACTICA 3
std::vector<Arbol *> ControlParques::listaArbParque(const std::string &asNum) {
    if (asNum.empty()) {
        throw std::invalid_argument("cadena asNum no valida");
    }
    Parque* parque = buscarParque(asNum);
    if (parque) {
        return parque->getArboles();
    }
    return std::vector <Arbol *>();
}


Arbol* ControlParques::buscarArbol(const std::string& asNum) {
    return trees.buscar(djb2(asNum.c_str()));
}

Arbol *ControlParques::arbolMasAlto() {
    if (idArboles.empty()) {
        throw std::runtime_error("No hay arboles cargados");
    }
    Arbol* aMasAlto = buscarArbol(idArboles[0]);
    for (unsigned int i = 1; i < idArboles.size(); i++) {
        Arbol* a = buscarArbol(idArboles[i]);
        if (a != nullptr && a->getAltura() > aMasAlto->getAltura()) {
            aMasAlto = a;
        }
    }
    return aMasAlto;
}

int ControlParques::contarTipos() {
    std::set<std::string> tiposDistintos;
    for (int i = 0; i < idArboles.size(); i++) { Arbol* a = buscarArbol(idArboles[i]);
        if (a != nullptr && !a->getTipoEspecie().empty()) {
            tiposDistintos.insert(a->getTipoEspecie());
        }
    }
    return tiposDistintos.size();
}



int ControlParques::contarEspecies(const std::string &nbreParque) const {
    if (nbreParque.empty()) {
        throw std::invalid_argument("Nombre de parque vacio");
    }
    for (std::map<int, Parque>::const_iterator it = parques.begin(); it != parques.end(); ++it) {
        if (it->second.get_nombre_parque() == nbreParque) {
            return it->second.contarEspecies();
        }
    }
    return 0;
}


Parque* ControlParques::buscarParqueCodigo(int cod) {
    std::map<int, Parque>::iterator it = parques.find(cod);
    if (it == parques.end()) {
        return nullptr;
    }
    return &(it->second);
}


Especie* ControlParques::buscarEspecieCodigo(const std::string& cod) {
    std::map<std::string, Especie>::iterator it = especies.find(cod);
    if (it == especies.end()) {
        return nullptr;
    }
    return &(it->second);
}

unsigned int ControlParques::numArboles() const {
    return trees.numElementos();
}


std::set<Parque*> ControlParques::listarParquesArbol(const std::string& nombreComun) {
    std::set<Parque*> resultado;
    std::pair<std::multimap<std::string, Arbol*>::iterator,
              std::multimap<std::string, Arbol*>::iterator> rango = treeName.equal_range(nombreComun);
    for (std::multimap<std::string, Arbol*>::iterator it = rango.first; it != rango.second; ++it) {
        Parque* p = it->second->getParque();
        if (p != nullptr) {
            resultado.insert(p);            // el set evita parques repetidos
        }
    }
    return resultado;
}

std::vector<Arbol*> ControlParques::buscarArbolNcomun(const std::string& nombreComun) {
    std::vector<Arbol*> resultado;
    std::pair<std::multimap<std::string, Arbol*>::iterator,
              std::multimap<std::string, Arbol*>::iterator> rango = treeName.equal_range(nombreComun);
    for (std::multimap<std::string, Arbol*>::iterator it = rango.first; it != rango.second; ++it) {
        resultado.push_back(it->second);
    }
    return resultado;
}


std::vector<Arbol*> ControlParques::buscarArbolNcomunComienzo(const std::string& comienzo) {
    if (comienzo.empty()) {
        throw std::invalid_argument("Nombre de comienzo vacio");
    }
    std::vector<Arbol*> resultado;
    std::multimap<std::string, Arbol*>::iterator it = treeName.lower_bound(comienzo);
    while (it != treeName.end() && it->first.compare(0, comienzo.size(), comienzo) == 0) {
        resultado.push_back(it->second);
        ++it;
    }
    return resultado;
}

int ControlParques::contarArbolesBarrio(int numBarrio)  {
    int contador = 0;
    for (unsigned int i =0; i< idArboles.size(); ++i) {
        Arbol*a = buscarArbol(idArboles[i]);
        if (a != nullptr && a->getNumBarrio() == numBarrio) {
            contador++;
        }

    }
    return contador;
}

//NUEVOS METODOS PRACTICA 5
unsigned long ControlParques::djb2(const char* str) const {
    unsigned long hash = 5381;
    char c;
    while ((c = *str++) != 0) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

std::vector<Arbol *> ControlParques::arbolesCajaDistrito(std::string &ndistrito) {

}

std::vector<CajaNido *> ControlParques::cajasEnDistrito(std::string &nbre_distrito) {

}

unsigned int ControlParques::contarCajasEnArbol() const {

}



