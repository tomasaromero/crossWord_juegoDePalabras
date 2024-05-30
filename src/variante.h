#ifndef TP_JUEGODEPALABRAS_VARIANTE_H
#define TP_JUEGODEPALABRAS_VARIANTE_H


#include "Tipos.h"
#include "Letra.h"
#include "string_map.h"

class Variante {
public:
    Variante(Nat n, Nat f, vector<Nat> puntajeLetras, string_map<bool> palabraLegitima);

    Nat tamanoTablero() const;

    Nat fichas() const;

    Nat puntajeLetra(Letra l) const;

    vector<Nat> puntajeLetras();

    bool palabraLegitima(const Palabra& palabra) const;
    //static bool palabraLegitma(const queue<Letra>& queue1);

private:
    Nat _tamanoTablero;

    Nat _fichas;

    vector<Nat> _puntajeLetras;

    string_map<bool> _palabraLegitima;
};

#endif //TP_JUEGODEPALABRAS_VARIANTE_H
