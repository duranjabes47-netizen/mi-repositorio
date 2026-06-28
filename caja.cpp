#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>

template <typename T> class caja {
private:
    std::allocator<T> alloc;
    //privado: no molestar
    //si no quieres ver mil errores

public:
    T value;
    T* datos;
    int capacidad;
    int tamano;

    //código horriblemente difícil de programar para

    //********** constructores allocator mode *****************

    caja(T valor) : value(valor), capacidad(5), tamano(0) {
        datos = alloc.allocate(capacidad);
    }

    caja()
        : capacidad(5),
          tamano(0)
    {
        datos = alloc.allocate(capacidad);
    }

    caja& operator=(caja&& otra) {
        if (this == &otra)
            return *this;

        for(int i = 0; i < tamano; i++) {
            alloc.destroy(&datos[i]);
        }
        alloc.deallocate(datos, capacidad);

        datos = otra.datos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;

        otra.datos = nullptr;
        otra.tamano = 0;
        otra.capacidad = 0;

        return *this;
    }

    caja(caja&& otra) {
        datos = otra.datos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;

        otra.datos = nullptr;
        otra.tamano = 0;
        otra.capacidad = 0;
    }

    caja& operator=(const caja& otra) {
        if(this == &otra)
            return *this;

        for(int i = 0; i < tamano; i++) {
            alloc.destroy(&datos[i]);
        }
        alloc.deallocate(datos, capacidad);

        tamano = otra.tamano;
        capacidad = otra.capacidad;

        datos = alloc.allocate(otra.capacidad);

        for(int i = 0; i < tamano; i++) {
            alloc.construct(&datos[i], otra.datos[i]);
        }
        return *this;
    }

    caja(const caja& otra) {
        tamano = otra.tamano;
        capacidad = otra.capacidad;

        datos = alloc.allocate(otra.capacidad);

        for(int i = 0; i < tamano; i++) {
            datos[i] = otra.datos[i];
        }
    }

    //********  funciones  ***********

    void push_back(const T& valor) {

        if (tamano <= capacidad) {
            reserve(capacidad * 2);
        }
        alloc.construct(&datos[tamano], valor);
        tamano++;
    }

    void mostrar() {
        for(int i = 0; i < tamano; i++) {
            std::cout << datos[i] << '\n';
        }
    }

    auto size() const {
        return tamano;
    }


    void reserve(int num) {

        T* datos2;
        std::allocator<T> alloc2;

        int tamano2 = 0;
        int capacidad2 = 0;

        capacidad2 = capacidad + num;
        datos2 = alloc.allocate(capacidad2);

        for (int i = 0; i < tamano; i++) {
            alloc.construct(&datos2[i], datos[i]);
        }

        for(int i = 0; i < tamano; i++) {
            alloc.destroy(&datos[i]);
        }

        alloc.deallocate(datos, capacidad);

        datos = datos2;
        capacidad = capacidad2;
    }


    int capacity() const {
        return capacidad;
    }


    T& at(int& num) {
        return datos[num];
    }

    auto erase(int& indice) {

        for(int i = indice; i < tamano - 1; i++) {
            datos[i] = datos[i + 1];
        }
        alloc.destroy(&datos[tamano-1]);
        tamano--;
    }

    T* begin() {
        return datos;
    }

    void insert(int& indice, const T& valor) {
        if (tamano == capacidad) {
            reserve(capacidad * 2);
        }
        for(int i = tamano; i > indice; i--) {
            datos[i] = datos[i - 1];
        }

        datos[indice] = valor;
        tamano++;
    }

    T* end() {
        return datos + tamano;
    }

    // *********  destructores. ************
    ~caja() {
        for(int i = 0; i < tamano; i++) {
            alloc.destroy(&datos[i]);
        }
        alloc.deallocate(datos, capacidad);
    }
};

int main() {

    caja<int> var;
    var.reserve(100);

    return 0;
}