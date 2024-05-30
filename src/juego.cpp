//
// Created by clinux01 on 25/11/22.
//
//#include "Fachada_Juego.h"

#include "juego.h"

//#include "variante.h"



Juego::Juego(Nat cantJug, const Variante& v,   Repositorio& r) :
        _variante(v),
        _tablero(v.tamanoTablero()),
        _turno(0),
        _ocurrencias( {} ),
        _fichas_jugadores( {} ),
        _puntaje_jugadores( {} )
{
    _repositorio =( r);
    _tablero = _variante.tamanoTablero();
    while( _fichas_jugadores.size() < cantJug  ) {
        _fichas_jugadores.emplace_back();
        _puntaje_jugadores.push_back({});
        _ocurrencias.emplace_back();
    }
    for (int i = 0; i < cantJug; ++i) {
        for (int j = 0; j < _variante.puntajeLetras().size(); ++j) {
            _fichas_jugadores[i].push_back(0);
        }
    }
    for (int id = 0; id < cantJug; ++id) {
        Nat cantFichas = _variante.fichas();
        for (int j = 0; j < cantFichas ; ++j) {
            _fichas_jugadores[id][ord(_repositorio.front())]++ ;
            _repositorio.pop_front();
        }
    }

}
bool Juego::jugadaValida( Ocurrencia&  o) {
    /*string_map<Letra> coordenadasO ;
    for(auto i=o.begin();i!=o.end();++i) {
        Palabra coord={};
        coord.push_back( (char) ord( get<0>(*i)%26 ) );
        coord.push_back( (char) ord( get<1>(*i)%26 ) );
        coordenadasO.insert( make_pair(coord,ord(get<1>(*i) )) );
    }*/

    for (auto i = o.begin(); i !=o.end() ; ++i) {
        Nat cuanto =_fichas_jugadores[ _turno ][ord(get<2>(*i))] ;
        bool noTiene=( cuanto == 0   );
        cout<<noTiene<<endl;
        if (hayLetra(get<0>(*i), get<1>(*i)) || noTiene) {
            return false;
        }

    }
    auto itRep = _repositorio.begin();
    list<Letra> headRep;
    for( int i = 0; i < o.size(); ++i ){
        headRep.push_back(*itRep);
        itRep++;
    }


    ubicar(o);//CUIDADO!!
    set<Ocurrencia> palabras= {};
    if ( o.empty() ) {
        return true;
    }
    if(o.size() == 1) {
        typename Ocurrencia:: iterator it = o.begin();
        set<Ocurrencia> palabras = palabrasUbicadas(o  );
        bool todasLegitim= todasLegitimas(palabras);
        return  todasLegitim;
    }
    bool palabraCoreIncluyeTodasFichas = true;
    typename Ocurrencia:: iterator it = o.begin();
    auto queEs=get<1>(*it);
    auto queEs2=get<1>(*(++it));
    if (get<1>(*it) == get<1>(*(++it))) { // Si es horizontal
        typename Ocurrencia:: iterator it = o.begin();
        Ocurrencia  oHorizontal = {};
        Nat prim = get<0>(*it);
        for (Nat i = get<0>(*it); _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)) ; --i) {
            prim = i;
        }
        Palabra coord={};
        for (Nat i = prim; _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)) ; ++i) {
            oHorizontal.insert({i, get<1>(*it), letra( i, get<1>(*it)) });
        }
        /*
        for( auto itO= o.begin(); itO!= o.end() ; itO++) {
            if ( not( pertenece(oHorizontal, get<2>(*itO)) ) ) {
                palabraCoreIncluyeTodasFichas = false;
            }
        }*/
        if ( not(pertenece(oHorizontal, o ) ) ) {
            palabraCoreIncluyeTodasFichas = false;
        }
        palabras = palabrasUbicadas( oHorizontal);
    } else {
        typename Ocurrencia :: iterator it= o.begin();
        Ocurrencia oVertical = {} ;
        Nat prim = get<1>(*it);
        for (Nat i = get<1>(*it); _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; --i) {
            prim = i;
        }
        for (Nat i = prim; _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; ++i) {
            oVertical.insert({get<0>(*it), i, letra(get<0>(*it), i) });
        }
        /*for( auto itO= o.begin(); itO!= o.end() ; itO++) {
            if ( not(pertenece(oVertical, get<2>(*itO) ) ) ) {
                palabraCoreIncluyeTodasFichas = false;
            }
        }*/
        if ( not(pertenece(oVertical, o ) ) ) {
            palabraCoreIncluyeTodasFichas = false;
        }
        palabras = palabrasUbicadas(oVertical);

    }

    bool todasLegitim= todasLegitimas(palabras);
    auto itLis = headRep.begin();
    while( itLis != headRep.end() ) {
        _repositorio.push_front(*itLis);//REVISAR ORDEN
        _fichas_jugadores[_turno][ord(*itLis)]--;
        //headRep.pop_front();
        itLis++;
    }

    desubicar(o);
    bool celdLib= celdasLibres( o);
    return ( celdLib && (palabraCoreIncluyeTodasFichas && todasLegitim));//celdasLibres falta hacer.
}
bool Juego::pertenece(Ocurrencia o, Ocurrencia jugada) {
    if ( o.size() < jugada.size() ) {
        return false;
    }

    //auto itO = jugada.begin();
    for (auto i = jugada.begin(); i  != jugada.end() ; ++i) {
        if ( o.count(*i)==0 ) {
            return false;
        }
    }
//  a ,    ,
//  a,    ,
//  a , b   , b
/*el pertenece esta mal
    for (auto i = o.begin(); i  != o.end() && j  != o.end() ; ++i) {
        for (auto j = jugada.begin(); j != jugada.end() &&  i  != o.end() ; ++j) {
            if (get<2>(*j) == get<2>(*i)) {
                jugada.erase(j);
            }
        }
    }//*/

    return true;
}
bool Juego::hayLetra(Nat x, Nat y) {
    return _tablero.hayLetra(x,y);
}

Letra Juego::letra(Nat i, Nat j) {
    return _tablero.letra(i,j);
}
void Juego::ubicar( const Ocurrencia& o) {
    _ocurrencias[_turno].push(o);
    int aQuienLeToca= _turno;
    for(typename Ocurrencia :: iterator itO= o.begin(); itO != o.end(); ++itO) {
        delete (_tablero.retTablero())[get<0>(*itO)][get<1>(*itO)];
        _tablero.ponerLetra(get<0>(*itO), get<1>(*itO), get<2>(*itO));
        Nat cantAnterior =  _fichas_jugadores[aQuienLeToca][ord(get<2>(*itO))];
        _fichas_jugadores[aQuienLeToca][ord(get<2>(*itO))] = cantAnterior - 1 ;
    }

    for(typename Ocurrencia :: iterator itO= o.begin(); itO != o.end(); ++itO) {
        Nat cantAnterior = _fichas_jugadores[aQuienLeToca][ord(_repositorio.front())];
        _fichas_jugadores[aQuienLeToca][ord(_repositorio.front())] = cantAnterior + 1;
        _repositorio.pop_front();
    }
    _turno = (_turno+1)%_puntaje_jugadores.size();
}
void Juego::desubicar(const Ocurrencia& o){
    _turno = (_turno-1)% _puntaje_jugadores.size();
    _ocurrencias[_turno].pop();
    for(typename Ocurrencia :: iterator itO= o.begin(); itO != o.end(); ++itO) {
        //_tablero[](  get<0>(*itO) )[](get<1>(*itO) )=nullptr ;/*
        //_tablero[0][0]= nullptr;*/
        //delete _tablero._tablero[get<0>(*itO)][get<1>(*itO)];
        //_tablero.puntero(get<0>(*itO), get<1>(*itO)) = nullptr;
        delete (_tablero.retTablero())[get<0>(*itO)][get<1>(*itO)];
        (_tablero.retTablero())[get<0>(*itO)][get<1>(*itO)] = nullptr;
    }
}

const Variante& Juego::variante() {
    return _variante;
}

IdCliente& Juego::turno() {
    return _turno;
}

Nat Juego::jugadores() {
    return _fichas_jugadores.size();
}

Nat  Juego::puntaje(IdCliente id) {
    Nat puntaje = _puntaje_jugadores[id];
    while(not( _ocurrencias[id].empty() )) {
        puntaje = puntaje + puntajePalabras( palabrasUbicadas( _ocurrencias[id].front()));
        _ocurrencias[id].pop();
    }
    return puntaje;
}

Nat Juego::cantFicha(IdCliente id, Letra l) {
    return _fichas_jugadores[id][ord(l)];
}

bool Juego::todasLegitimas(set<Ocurrencia> os){
    bool res = true;
    typename set<Ocurrencia>::iterator itOs = os.begin();
    for (typename set<Ocurrencia>::iterator itOs = os.begin(); itOs!=os.end(); ++itOs) {
        typename Ocurrencia::iterator itO = itOs->begin();
        queue<Letra> colaLetras = {};
        if (get<1>(*itO) == get<1>(*(itO++))) {
            typename Ocurrencia::iterator itO = itOs->begin();
            Nat prim = get<0>(*itO);
            for (Nat i = get<0>(*itO); _tablero.enTablero(i, get<1>(*itO)) && _tablero.hayLetra(i, get<1>(*itO)) ; --i) {
                prim = i;
            }
            for (Nat i = prim; _tablero.enTablero(i, get<1>(*itO)) && _tablero.hayLetra(i, get<1>(*itO)) ; ++i) {
                colaLetras.push(letra(i, get<1>(*itO)));
            }
        } else {
            typename Ocurrencia::iterator itO = itOs->begin();
            Nat prim = get<1>(*itO);
            for (Nat i = get<1>(*itO); _tablero.enTablero(get<0>(*itO), i) && _tablero.hayLetra(get<0>(*itO), i) ; --i) {
                prim = i;
            }
            for(Nat i = prim; _tablero.enTablero(get<0>(*itO), i) && _tablero.hayLetra(get<0>(*itO), i) ; ++i) {
                colaLetras.push(letra(get<0>(*itO), i));
            }
        }
        Palabra palabra = {};
        while( colaLetras.empty()==false ) {
            palabra.push_back(colaLetras.front());
            colaLetras.pop();
        }

        if ( not( _variante.palabraLegitima(palabra)  ) ) {//colaLetra to vector(Letra)
            res = false;
        }
    }
    return res;
}


set<Ocurrencia> Juego:: palabrasUbicadas(Ocurrencia& o) {
    if ( o.empty() ) {
        return {};
    }
    set<Ocurrencia> res = {};
    bool uno = o.size()==1;
    if ( (uno) ) {//

        Ocurrencia  oHorizontal = {};
        auto itO = (o).begin();

        Nat prim = get<0>(*itO);

        for(Nat i = get<0>(*itO); _tablero.enTablero(i, get<1>(*itO)) && _tablero.hayLetra(i, get<1>(*itO)); --i ) {
            prim= i;
        }


        for(Nat i = prim; _tablero.enTablero(i, get<1>(*itO)) && _tablero.hayLetra(i, get<1>(*itO)); ++i ) {
            oHorizontal.insert({i, get<1>(*itO), letra(i, get<1>(*itO))});
        }
        res = {};
        res.insert(oHorizontal);

        Ocurrencia oVertical = {};
        itO = (o).begin();
        prim = get<1>(*itO);
        Nat columna = get<0>(*itO);
        Nat fila = get<1>(*itO);
        //cout << "llegue" << endl;

        for(Nat i = fila; _tablero.enTablero(columna, i) && _tablero.hayLetra(columna , i) ; --i) {
            cout << i << endl;
            prim= i;
        }
        for (Nat i = prim; _tablero.enTablero(get<0>(*itO) , i) && _tablero.hayLetra(get<0>(*itO) , i) ; ++i) {
            //cout << "Insertando letra: " << letra( get<0>(*itO), i) << endl;
            oVertical.insert({get<0>(*itO), i, letra( get<0>(*itO), i) });
        }

        if (res.count(oVertical)==0) {
            res.insert(oVertical);
        }

    } else {
        typename Ocurrencia ::iterator it = o.begin();
        if (get<1>(*it)==get<1>(*(++it)) ) {
            palabrasUbicadasOHorizontal(o, res);
        } else {
            palabrasUbicadasOVertical(o, res);
        }

    }
    return res;
}
void Juego::palabrasUbicadasOHorizontal(Ocurrencia& o, set<Ocurrencia>& res) {
    auto it = o.begin();
    Ocurrencia oHorizontal={};
    Nat prim = get<0>(*it);
    for(Nat i = get<0>(*it); _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)); --i ) {
        prim= i;
    }
    for(Nat i = prim; _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)); ++i ) {
        oHorizontal.insert({i, get<1>(*it), letra(i, get<1>(*it))});
    }
    res.insert(oHorizontal);
    while(  it != o.end() ){
        Ocurrencia  oVertical ={};
        prim = get<1>(*it);
        for(Nat i = get<1>(*it); _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; --i) {
            prim= i;
        }
        for (Nat i = prim; _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; ++i) {
            oVertical.insert({get<0>(*it), i, letra( get<0>(*it), i) });
        }
        res.insert(oVertical);
        it++;
    }
}
void Juego::palabrasUbicadasOVertical(Ocurrencia& o, set<Ocurrencia>& res) {
    auto it = o.begin();
    Ocurrencia oVertical = {};
    Nat prim =get<1>(*it);
    for(Nat i = get<1>(*it); _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; --i) {
        prim= i;
    }
    for (Nat i = prim; _tablero.enTablero(get<0>(*it) , i) && _tablero.hayLetra(get<0>(*it) , i) ; ++i) {
        oVertical.insert({get<0>(*it), i, letra( get<0>(*it), i) });
    }
    res.insert(oVertical);
    while( it != o.end() ) {
        Ocurrencia oHorizontal={};
        Nat prim =get<0>(*it);
        for(Nat i = get<0>(*it); _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)); --i ) {
            prim= i;
        }
        for(Nat i = prim; _tablero.enTablero(i, get<1>(*it)) && _tablero.hayLetra(i, get<1>(*it)); ++i ) {
            oHorizontal.insert({i,get<1>(*it), letra(i, get<1>(*it))});
        }
        res.insert(oHorizontal);
        it++;
    }

}
Nat Juego :: puntajePalabras( set<Ocurrencia> os ) {
    Nat res = 0;
    if ( os.empty() ) {
        return res;
    }
    auto itOs = os.begin();
    while( itOs != os.end()) {
        res = res + puntajePalabra( *itOs ) ;
        itOs++;
    }
    auto it = os.begin();
    if( os.size()==1 && (*it).size()==1 ) {
        return res*2;
    }else {
        return res;
    }
}
Nat Juego:: puntajePalabra(Ocurrencia o ) {
    Nat res = 0;
    if ( o.empty() ) {
        return res;
    }
    auto itO = o.begin();
    while( itO != o.end() ) {
        res = res + _variante.puntajeLetra( get<2>(*itO) );
        itO++;
    }
    return  res;
}

Tablero Juego::tablero() {
    return _tablero;
}
vector<vector<Nat>>& Juego:: fichas_jugadores() {
    return _fichas_jugadores;
}
Repositorio Juego::repositorio() {//  revisar: repo de queue(Letra) a list(Letra)
    return _repositorio;
}

bool Juego::celdasLibres( Ocurrencia& o) {
    bool libres= true;
    for (auto  it = o.begin(); it != o.end() ; ++it) {
        if ( _tablero.hayLetra(get<0>(*it), get<1>(*it) ) )
            libres = false;
    }
    return libres;
}
