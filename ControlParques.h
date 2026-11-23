//
// Created by a_amo on 07/10/2026.
//

#ifndef PRACTICA_3_CONTROLPARQUES_H
#define PRACTICA_3_CONTROLPARQUES_H


#include <string>
#include "Especie.h"
#include "Parque.h"
#include "ThashArbol.h"
#include "Arbol.h"
#include <vector>
#include <map>
#include <set>
#include "CajaNido.h"


class ControlParques {
private:
    std::map<std::string, Especie> especies;
    std::map<int, Parque> parques;
    ThashArbol trees;
    std::multimap <std::string, Arbol*> treeName;
    std::vector<CajaNido> nests;
    std::vector<std::string> idArboles;
public:
    ControlParques(const std::string& rutaEspecies, const std::string& rutaParques, const std::string& rutaArboles);
    std::map<std::string,Especie>& get_especies();
    std::map<int ,Parque>& get_parques();

    //Metodos del UML
    int contarEspecieParque(const std::string& nComun);

    void asignarEspecieParque(Especie* e, Parque& p);


    Parque* buscarParque(const std::string& nombreParque);


    std::map<std::string, Especie*> listadoEspeciesSubNbre(const std::string& cadena);

    //UML PRACTICA 3
    std::vector<Arbol*>listaArbParque(const std::string& asNum);


    Arbol* buscarArbol(const std::string& asNum);

    Arbol* arbolMasAlto() ;

    int contarTipos();

    int contarEspecies(const std::string& nbreParque) const;


    //UML PRACTICA 4
    std::set<Parque*> listarParquesArbol(const std::string& nombreComun);


    std::vector<Arbol*> buscarArbolNcomun(const std::string& nombreComun);


    std::vector<Arbol*> buscarArbolNcomunComienzo(const std::string& comienzo);


    int contarArbolesBarrio(int numBarrio);

    Parque* buscarParqueCodigo(int cod);


    Especie* buscarEspecieCodigo(const std::string& cod);


    unsigned int numArboles() const;


    //NUEVOS METODOS PRACTICA 5
    unsigned long djb2(const char* str) const;
    unsigned int contarCajasEnArbol() const;
    std::vector<CajaNido*> cajasEnDistrito(std::string &nbre_distrito);
    std::vector<Arbol*> arbolesCajaDistrito(std::string &ndistrito);

};


#endif //PRACTICA_3_CONTROLPARQUES_H