#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H
#include "Tipos.h"
//#include "variante.h"
//#include "Juego.h"
#include "Fachada_Juego.h"
//#include "Fachada_Servidor.h"
#include "Notificacion.h"



class Servidor {
public:
    Servidor(Nat esperados, const Variante& v, Repositorio& r);

    bool empezo() const;

    IdCliente conectarCliente();

    void recibirMensaje(IdCliente id, const Ocurrencia &o);

    Nat jugadoresEsperados();

    Nat jugadoresConectados();

    Juego juego();

    list <Notificacion> notificaciones(IdCliente id);


private:
    Juego _j;
    Nat _jugadoresEsperados;
    Nat _jugadoresConectados;
    tuple <Variante, Repositorio> _configuracion;

    vector <list<tuple < Nat, Notificacion>>> _notificacionesId;
    list <tuple<Nat, Notificacion>> _notificacionesGen;
    Nat _contadorNotif;
    vector <Nat> _ultimaConsulta;

    queue<Notificacion> consultar(IdCliente id);

};
#endif //TP_JUEGODEPALABRAS_SERVIDOR_H
