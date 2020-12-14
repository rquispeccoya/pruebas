#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>

using namespace std;
//creamos la clase NODO
class Nodo{
private:
    //establecemos los atributos
    Nodo *sig;
    uint16_t elem;
public:
    //creamos el constructor explicito
    Nodo(Nodo *a,uint16_t e):sig{a},elem{e}{}
    //Creamos los metodos get y set
    uint16_t getElemento(){ return elem;}
    Nodo* getSiguiente(){return sig;}
    void setElemento(uint16_t e){this->elem=e;}
    void setSiguiente(Nodo *n){this->sig=n;}
};

//creamos la clase Lista
class Lista{
private:
    Nodo *cabeza;
    unsigned numElem=0;
    std::mutex mi; //creamos nuestro semaforo

public:
    //creamos el metodo de insertar elemento

    void insertarElemento(){
        srand(time(NULL));
        //establecemos el bucle que insertara 10 elementos
        for(int i=0;i<10;i++){
            //bloquemos la seccion critica
            mi.lock();
            //generamos numero aleatorio entre 0-14
            int elem=rand()%15;
            //creamos el nuevo nodo a insertar
            Nodo *a=new Nodo(nullptr,elem);
            //verificamos si la lista esta vacia
            if(this->cabeza==nullptr){
                cabeza=a;
                ++numElem;
            }
            else{
                //creamos un nodo auxiliar
                Nodo *aux=cabeza;
                //recorremos al final de la lista
                while(aux->getSiguiente() != nullptr){
                    aux=aux->getSiguiente();
                }
                //insertamos el nuevo nodo al final de la lista
                aux->setSiguiente(a);
                ++numElem;
            }
            cout<<"Insertando: "<<elem<<" en la posicion "<<numElem<<endl;
            //desbloqueamos
            mi.unlock();
            //el proceso espera 100 miliseg
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void mostrarLista(){
        //creamos un nodo auxiliar
        Nodo * aux=cabeza;
        while(aux!=nullptr){
            //imprime el elemento
            cout<<aux->getElemento()<<" --> ";
            //avanza al siguiente nodo
            aux=aux->getSiguiente();
        }
    }

    //metodo que elimina un elemento
    void eliminarElemento(){
        srand(time(NULL));
        //establecemos el bucle que eliminara 10 elementos
        for(int i=0;i<10;i++){
            //bloqueamos la seccion critica
            mi.lock();
            bool encontrado=false;
            //generamos un numero aleatorio a eliminar
            uint16_t elem=rand()%15;
            //verificamos si la lista esta vacia
            if(cabeza==nullptr)
                cout<<"No eliminado LISTA VACIA"<<endl;
            else{
                //creamos un nodo auxiliar
                Nodo *aux=cabeza;
                //creamos un nodo que apunta al nodo anterior
                Nodo *ant=nullptr;
                while(aux!=nullptr){
                    if(aux->getElemento()==elem){
                        //si se encuentra el elemento
                        //verificamos si esta al inicio
                        if(aux==cabeza)
                            cabeza=cabeza->getSiguiente();
                        //sino esta al inicio entonces entra al else
                        else
                            ant->setSiguiente(aux->getSiguiente());
                        //cuando encuentra el elemento es true
                        encontrado=true;
                        //sale del bucle
                        --numElem;break;
                    }
                    ant=aux;
                    aux=aux->getSiguiente();
                }
                //verificamos si se encontro o no
                if(encontrado)
                    cout<<"Eliminado: "<<elem<<endl;
                else
                    cout<<"No se elimino: "<<elem<<endl;
            }
            //desbloqueamos
           mi.unlock();
           //el proceso espera 100 miliseg
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    //metodo que buscara un elemento
    void buscarElemento(){
        srand(time(NULL));
        //establecemos el bucle que buscara 10 elementos
        for(int i=0;i<10;i++){
            //bloqueamos la seccion critica
            mi.lock();
            bool encontrado=false;
            //generamos un numero aleatorio a buscar
            uint16_t elem=rand()%15;
            //verificamos si la lista esta vacia
            if(cabeza==nullptr)
                cout<<"No encontrado LISTA VACIA"<<endl;
            else{
                Nodo *aux=cabeza;
                while(aux!=nullptr){
                    //si se encuentra el elemento
                    //usamos break para salir del bucle
                    if(aux->getElemento()==elem){
                        encontrado=true;break;
                    }
                    else{
                        //sino se encuentra
                        //avanza al siguiente nodo
                        aux=aux->getSiguiente();
                    }
                }
                //evaluamos si es encuentra
                if(encontrado)
                    cout<<"Encontrado: "<<elem<<endl;
                else
                    cout<<"No encontrado: "<<elem<<endl;
            }
            mi.unlock();
            //el proceso espera 100 miliseg
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    //metodo que modificara un elemento
    void modificarElemento(){
        srand(time(NULL));

        for(int i=0;i<10;i++){
            //bloqueamos la seccion critica
            mi.lock();
            bool encontrado=false;
            //generamos aleatoriamente el numero a modificar
            uint16_t elem=rand()%15;
            //generamos aleatoriamente la cantidad a sumar
            uint16_t num=5+rand()%5;
            //
            if(cabeza==nullptr)
                cout<<"No modificado LISTA VACIA"<<endl;
            else{
                Nodo *aux=cabeza;
                while(aux!=nullptr){
                    //cuando se encuentra el elemento se modifica
                    if(aux->getElemento()==elem){
                        //sumamos al elemento encontrado
                        num=elem+num;
                        //modificamos el elemento
                        aux->setElemento(num);
                        encontrado=true;break;
                    }
                    else{
                        aux=aux->getSiguiente();
                    }
                }
                //verificamos si se encontro el elemento a modificar
                if(encontrado)
                    cout<<"Se modifico de : "<<elem<<" a "<<num<<endl;
                else
                    cout<<"No se modifico "<<endl;
            }
            mi.unlock();
            //el proceso espera 100 miliseg
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};
int main()
{
    //creamos una instancia de la clase lista
    Lista *a=new Lista();
    //creamos nuestros 4 hilos
    //eliminar,insertar,buscar y modificar
    std::thread pr1(&Lista::eliminarElemento,*&a);
    std::thread pr2(&Lista::insertarElemento,*&a);
    std::thread pr3(&Lista::buscarElemento,*&a);
    std::thread pr4(&Lista::modificarElemento,*&a);
    pr1.join();
    pr2.join();
    pr3.join();
    pr4.join();
    //mostramos la lista despues de toda las operaciones
    a->mostrarLista();
    cout<<endl;
}
