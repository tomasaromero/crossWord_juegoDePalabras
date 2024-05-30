#include "variante.h"

Variante::Variante(Nat n, Nat f, vector<Nat> puntajeLetras, string_map<bool> palabraLegitima) : _tamanoTablero(n),
_fichas(f), _puntajeLetras(puntajeLetras), _palabraLegitima(palabraLegitima){}

Nat Variante::tamanoTablero() const {
    return _tamanoTablero;
}

Nat Variante::fichas() const {
    return _fichas;
}

Nat Variante::puntajeLetra(Letra l) const {
    return _puntajeLetras[ord(l)];
}

vector<Nat> Variante::puntajeLetras() {
    return _puntajeLetras;
}

bool Variante::palabraLegitima(const Palabra& palabra) const{
    return _palabraLegitima.count(palabra) == 1;
}