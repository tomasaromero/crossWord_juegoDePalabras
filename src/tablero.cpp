#include "tablero.h"

Tablero::Tablero(Nat n){
    _tamano = n;
    Nat i = 0;
    while (i < n) {
        Nat j = 0;
        vector<Letra*> fila;
        while (j < n) {
            fila.push_back(nullptr);
            j++;
        }
        _tablero.push_back(fila);
        i++;
    }
}

Tablero::~Tablero() {
    for (Nat i = 0; i < _tamano; i++) {
        for (Nat j = 0; j < _tamano; j++) {
            if (_tablero[i][j] != nullptr) {
                delete _tablero[i][j];
            }
        }
    }
}

void Tablero::ponerLetra(Nat i, Nat j, Letra l) {
    _tablero[i][j] = new Letra (l);
}

Nat Tablero::tamano() {
    return _tamano;
}

bool Tablero::hayLetra(Nat i, Nat j) {
    return _tablero[i][j] != nullptr;
}

Letra Tablero::letra(Nat i, Nat j) {
    return *(_tablero[i][j]);
}

bool Tablero::enTablero(Nat i, Nat j) {
    return i < _tamano && j < _tamano;
}

Letra*& Tablero::puntero(Nat i, Nat j)& {
    return  _tablero[i][j];
}

vector<vector<Letra*>>& Tablero::retTablero() {
    return  _tablero;
}