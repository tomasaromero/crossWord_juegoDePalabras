
#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H
#include "tablero.h"
#include "Tipos.h"
#include "variante.h"


class Juego {
public:
    Juego(Nat cantJug, const Variante& v,  Repositorio& r);
    //Juego(Nat cantJug, const Variante &v, const Repositorio &r);

    void ubicar(const Ocurrencia& o);
    IdCliente& turno();
    const Variante& variante();
   //bool jugadaValida(const Ocurrencia& o);
    bool jugadaValida( Ocurrencia& o);
    bool hayLetra(Nat x, Nat y);
    Letra letra(Nat i, Nat j);
    Nat puntaje(IdCliente id);
    Nat cantFicha(IdCliente id, Letra l);
    Tablero tablero();
    vector<vector<Nat>>& fichas_jugadores();
    Repositorio repositorio();
    Nat jugadores();
    Nat puntajePalabras( set<Ocurrencia> os );
    Nat puntajePalabra( Ocurrencia o );
    set<Ocurrencia> palabrasUbicadas( Ocurrencia& o );

private:
    Tablero _tablero;
    vector<vector<Nat>> _fichas_jugadores;
    vector<Nat> _puntaje_jugadores;
    Nat _turno;
    Repositorio _repositorio;
    Variante _variante;
    vector<queue<Ocurrencia>> _ocurrencias;
    bool todasLegitimas( set<Ocurrencia> os);
    void palabrasUbicadasOHorizontal( Ocurrencia& o,  set<Ocurrencia>& res );
    void palabrasUbicadasOVertical( Ocurrencia& o ,  set<Ocurrencia>& res );
  //Nat puntajePalabra( Ocurrencia o );
  //Nat puntajePalabras( set<Ocurrencia> os );
    bool celdasLibres(  Ocurrencia& o);
    bool pertenece( Ocurrencia o ,  Ocurrencia jugada );
    void desubicar(const Ocurrencia& o);
};



#endif //TP_JUEGODEPALABRAS_JUEGO_H
