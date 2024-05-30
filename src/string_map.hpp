template <typename T>
string_map<T>::string_map() : raiz(new Nodo()), _size(0){
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    for (int i = 0; i < d._paresDefinidos.size(); i++) {
        insert(d._paresDefinidos[i]);
    }
}

template <typename T>
string_map<T>::~string_map(){
    _destructor(raiz);
}

template <typename T>
void string_map<T>::insert(const pair<Palabra, T> & par) {
    Nodo* actual = raiz;
    Palabra clave = par.first;
    T valor = par.second;
    T* copiaValor = new T (valor);
    int i = 0;
    while (i < clave.size() - 1) {
        Nat indiceLetra = ord(clave[i]);
        if (actual->siguientes[indiceLetra] == nullptr) {
            actual->siguientes[indiceLetra] = new Nodo();
            actual = actual->siguientes[indiceLetra];
        } else {
            actual = actual->siguientes[indiceLetra];
        }
        i++;
    }
    Nat indiceLetra = ord(clave[i]);
    if (actual->siguientes[indiceLetra] == nullptr) {
        actual->siguientes[indiceLetra] = new Nodo(copiaValor);
        _size++;
    } else {
        actual = actual->siguientes[indiceLetra];
        delete actual->definicion;
        actual->definicion = copiaValor;
    }
    _paresDefinidos.push_back(par);
}

template <typename T>
int string_map<T>::count(const Palabra& clave) const{
    Nodo* actual = raiz;
    int i = 0;
    while (i < clave.size()) {
        Nat indiceLetra = ord(clave[i]);
        if (actual->siguientes[indiceLetra] == nullptr) {
            return 0;
        } else {
            actual = actual->siguientes[indiceLetra];
            i++;
        }
    }
    if (actual != nullptr && actual->definicion != nullptr) {
        return 1;
    } else {
        return 0;
    }
}

template <typename T>
T& string_map<T>::at(const Palabra& key) {
    Nodo* actual = raiz;
    int i = 0;
    while (i < key.size()) {
        Nat indiceLetra = ord(key[i]);
        actual = actual->siguientes[indiceLetra];
        i++;
    }
    return *(actual->definicion);
}