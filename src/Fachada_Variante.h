#ifndef FACHADA_VARIANTE_H
#define FACHADA_VARIANTE_H

#include "Letra.h"
#include "variante.h"

class Fachada_Variante {
public:
    /**
     * Construye una Fachada_Variante a partir de los parametros de las variantes
     */
    Fachada_Variante(
            Nat tamanoTab,
            Nat cantFichas,
            const map<Letra, Nat>& puntajes,
            const set<vector<Letra>>& palabrasLegitimas
    );

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat tamanoTablero() const;

    /**
     * Retorna la cantidad de fichas que cada jugador deberia tener en su mano.
     */
    Nat fichas() const;

    /**
     * Retorna el puntaje de la letra l
     */
    Nat puntajeLetra(Letra l) const;

    /**
     * Indica si una palabra es legitima o no
     */
    bool palabraLegitima(const Palabra& palabra) const;

private:
    Variante _variante;

    vector<Nat> _map2Vector(map<Letra, Nat> dicc) {
        vector<Nat> res(TAMANIO_ALFABETO);
        for (Nat i = 0; i < TAMANIO_ALFABETO; i++) {
            Letra l = inversaDeOrd(i);
            if (dicc.count(l) == 1) {
                res[i] = dicc[l];
            } else {
                res[i] = 1;
            }
        }
        return res;
    }

    string_map<bool> _set2Trie(set<vector<Letra>> palabrasLegitimas) {
        string_map<bool> res;
        for (vector<Letra> p : palabrasLegitimas) {
            res.insert(make_pair(p, true));
        }
        return res;
    }
};

#endif //FACHADA_VARIANTE_H
