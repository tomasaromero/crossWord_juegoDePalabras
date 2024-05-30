//
// Created by clinux01 on 25/11/22.
//
#include "servidor.h"
#include "Tipos.h"

Servidor::Servidor(Nat esperados, const Variante& v, Repositorio& r) :
        _j(Juego(esperados, v, r)),
        _jugadoresConectados(0),
        _jugadoresEsperados(esperados),
        _configuracion(make_tuple(v, r))
{
    vector<list<tuple<Nat, Notificacion>>> notifId;
    list<tuple<Nat,Notificacion>> notifGen;
    vector<Nat> ultimaConsulta;
    for (int i = 0; i < esperados; i++) {
        notifId.push_back({});//push||emplace
        ultimaConsulta.push_back(0);
    }
    _notificacionesId = notifId;
    _notificacionesGen = notifGen;
    _ultimaConsulta = ultimaConsulta;
    _contadorNotif = 0;
}
bool Servidor::empezo() const {
    return _jugadoresEsperados == _jugadoresConectados;
}

//IdCliente Servidor::conectarCliente() {
//    _notificacionesId[_jugadoresConectados].push_front(make_tuple(_contadorNotif,Notificacion::nuevaIdCliente(_jugadoresConectados)));
//    _contadorNotif++;
//    if (not(empezo())&& (_jugadoresEsperados==_jugadoresConectados+1)){
//        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaEmpezar(get<0>(_configuracion).tamanoTablero())));
//        _contadorNotif++;
//        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaTurnoDe(0)));
//        _contadorNotif++;
//    }
//    _jugadoresConectados++;
//    return _jugadoresConectados-1;
//}

IdCliente Servidor::conectarCliente() {
    _notificacionesId[_jugadoresConectados].push_front(make_tuple(_contadorNotif,Notificacion::nuevaIdCliente(_jugadoresConectados)));
    _contadorNotif++;
    _jugadoresConectados++;
    if (empezo()){
        _notificacionesGen.push_front(
                make_tuple(_contadorNotif, Notificacion::nuevaEmpezar(get<0>(_configuracion).tamanoTablero())));
        _contadorNotif++;
        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaTurnoDe(0)));
        _contadorNotif++;
        auto itRep = (get<1>(_configuracion)).begin();
        Nat cantFichas = (get<0>(_configuracion)).fichas();
        for (int id = 0; id < _jugadoresConectados; id++) {
            multiset<Letra> fichas={};
            for (int j = 0; j < cantFichas ; ++j) {
                fichas.insert(*itRep);
                itRep++;
            }
            _notificacionesId[id].push_back(
                    make_tuple(_contadorNotif, Notificacion::nuevaReponer(fichas)));
            _contadorNotif++;
        }
    }
    return _jugadoresConectados-1;
}

queue<Notificacion> Servidor::consultar(IdCliente id) {
    list <tuple<Nat,Notificacion>> notifGen;
    // agregué un if si _notificacionesGen está vacía
    if (not _notificacionesGen.empty()) {
        list <tuple<Nat,Notificacion>> ::iterator itList = _notificacionesGen.begin();
        for (int i = _ultimaConsulta[id]; i < _contadorNotif && itList != _notificacionesGen.end(); i++) {
            notifGen.push_front((*itList)); // Saqué el _1
            ++itList;
        }
    }
    _ultimaConsulta[id] = _contadorNotif + 1;
    list <tuple<Nat, Notificacion>> notifId = _notificacionesId[id];
    vector <tuple<Nat, Notificacion>> arrNotifId;
    list < tuple < Nat, Notificacion >> ::iterator
            itLid = notifId.begin();
    while (itLid != notifId.end()) {
        arrNotifId.push_back(*itLid);
        ++itLid;
    }
    vector <tuple<Nat, Notificacion>> arrNotifGen;
    list < tuple < Nat, Notificacion >> ::iterator
            itLg = notifGen.begin();
    while (itLg != notifGen.end()) {
        arrNotifGen.push_back(*itLg);
        ++itLg;
    }
    vector < tuple < Nat, Notificacion >> ::iterator
            itVid = arrNotifId.begin();
    vector < tuple < Nat, Notificacion >> ::iterator
            itVg = arrNotifGen.begin();
    queue <Notificacion> res;
    while (itVid != arrNotifId.end() && itVg != arrNotifGen.end()) { //Cambié el o por y
        if (get<0>(*itVid) < get<0>(*itVg)) {
            res.push(get<1>(*itVid));
            ++itVid;
        } else {
            res.push(get<1>(*itVg));
            ++itVg;
        }
    }
    while (itVid != arrNotifId.end()) {
        res.push(get<1>(*itVid));
        ++itVid;
    }
    while (itVg != arrNotifGen.end()) {
        res.push(get<1>(*itVg));
        ++itVg;
    }/* vacía todas las notificaciones
    vector < list < tuple < Nat, Notificacion>>> notifIdvacia;
    for (int i = 0; i < _jugadoresConectados; i++) {
        notifIdvacia.push_back({});
    }*/
    _notificacionesId[id] = {}; // vacía solo las notificaciones del cliente pedido
    return res;
}


list<Notificacion> Servidor::notificaciones(IdCliente id){
    queue<Notificacion> colaNotif = consultar(id);
    list<Notificacion> res;
    while( not(colaNotif.empty()) ){
        res.push_back(colaNotif.front());
        colaNotif.pop();
    }
    return res;
}



void Servidor::recibirMensaje(IdCliente id, const Ocurrencia& o){
    //Juego j=_j;
    if (empezo() && _j.turno() == id && _j.jugadaValida( (Ocurrencia&) o ) ) {
        multiset<Letra> fichas={};
        //Repositorio::iterator itRep = (get<1>(_configuracion)).begin();
        // p = _j.repositorio().begin();
        Repositorio sacadas ={};
        Repositorio copia = _j.repositorio();
        auto itRep= copia.begin();
        for (auto itO = o.begin(); itO != o.end(); ++itO) {
            fichas.insert(*itRep);
            ++itRep;
            //_j.fichas_jugadores()[(_j.turno()- 1) % _j.fichas_jugadores().size()][ord(get<2>(*itO))]--;//-----------

            //fichas.insert( _j.repositorio().front() );
            //sacadas.push_back(_j.repositorio().front());
            //_j.repositorio().pop_front();

            //_j.fichas_jugadores()[(_j.turno()-1) % _j.fichas_jugadores().size()][ord(*itRep)]++;
            //_j.repositorio().pop_front();//---------------
            // ++;
        }
        /*
        while(not(sacadas.empty()) ) {
            _j.repositorio().push+_front(sacadas.front());
            sacadas.pop_front();
        }*/

        _j.ubicar(o);
        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaUbicar(id, o)));
        _contadorNotif++;
        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaSumaPuntos(id, _j.puntajePalabras(_j.palabrasUbicadas( (Ocurrencia&) o))))); // Cambié a puntajePalabra
        _contadorNotif++;
        _notificacionesId[id].push_back(make_tuple(_contadorNotif, Notificacion::nuevaReponer(fichas)));
        _contadorNotif++;
        _notificacionesGen.push_front(make_tuple(_contadorNotif, Notificacion::nuevaTurnoDe((id + 1)  % _jugadoresEsperados)));
        _contadorNotif++;
    }
    else {
        _notificacionesId[id].push_back(make_tuple(_contadorNotif,Notificacion::nuevaMal()));
        _contadorNotif++;
    }
}

Nat Servidor::jugadoresEsperados(){
    return _jugadoresEsperados;
}

Nat Servidor::jugadoresConectados(){
    return _jugadoresConectados;
}

Juego Servidor::juego(){
    return _j;
}
