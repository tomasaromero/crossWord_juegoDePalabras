#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include "Tipos.h"

using namespace std;

template<typename T>
class string_map {
public:
    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    **/
    string_map();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    string_map(const string_map<T>& aCopiar);

    /**
    OPERADOR ASIGNACION
     */
    string_map& operator=(const string_map& d);

    /**
    DESTRUCTOR
    **/
    ~string_map();

    /**
    INSERT 
    * Inserta un par clave, valor en el diccionario
    **/
    void insert(const pair<Palabra, T>&);

    /**
    COUNT
    * Devuelve la cantidad de apariciones de la clave (0 o 1).
    * Sirve para identificar si una clave está definida o no.
    **/

    int count(const Palabra &clave) const;

    T& at(const Palabra& key);//

private:

    struct Nodo {
        vector<Nodo*> siguientes;
        T* definicion;
        Nodo() : siguientes(26, nullptr), definicion(nullptr) { }
        Nodo(T* def) : siguientes(26, nullptr), definicion(def) { }
    };

    Nodo* raiz;
    int _size;

    void _destructor(Nodo* n) {
        vector<int> definidos = _definidosEnNodo(n);
        if (!definidos.empty()) {
            for (int i = 0; i < definidos.size(); i++) {
                int indice = definidos[i];
                _destructor(n->siguientes[indice]);
            }
        }
        delete n->definicion;
        delete n;
    }

    vector<int> _definidosEnNodo(Nodo* n) {
        int i = 0;
        vector<int> cantidad;
        while (i < 26) {
            if (n != nullptr && n->siguientes[i] != nullptr) {
                cantidad.push_back(i);
            }
            i++;
        }
        return cantidad;
    }

    vector<pair<Palabra, T>> _paresDefinidos;
};

#include "string_map.hpp"

#endif // STRING_MAP_H_
