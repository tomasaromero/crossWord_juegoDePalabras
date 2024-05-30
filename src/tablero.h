#ifndef TP_JUEGODEPALABRAS_TABLERO_H
#define TP_JUEGODEPALABRAS_TABLERO_H

#include "Tipos.h"
#include "Letra.h"

class Tablero {
public:
    Tablero(Nat n);

    ~Tablero();

    void ponerLetra(Nat i, Nat j, Letra l);

    Nat tamano();

    bool hayLetra(Nat i, Nat j);

    Letra letra(Nat i, Nat j);

    bool enTablero(Nat i, Nat j);

    Letra*& puntero(Nat i, Nat j)&;

    vector<vector<Letra*>>& retTablero();

private:
    Nat _tamano;

    vector<vector<Letra*>> _tablero;
};

#endif //TP_JUEGODEPALABRAS_TABLERO_H
