#include <iostream>
#include <vector>
#include <map>
#include "contigua.hpp"

namespace contigua{
    //variaveis internas
    static std::vector<int> memoria; //static para evitar conflito e garantir usabilidade em todo o programa - vector represntando celulas da memoria
    static std::map<int, Processo> tabela; //map representa tabela de processos

    void inicMemoria(int n){
        if(n<=0){
            n=50;
        }
        memoria.assign(n, -1);
        tabela.clear();
        std::cout << "Memoria incializada com " << n <<  " unidades.\n";
    }

    void mostrarMemoria(){
        if(memoria.empty()){
            std::cout << "Memoria não inicializada\n";
            return;
        }
        std::cout << "Memoria (" << memoria.size() << " unidades):\n";
        for(size_t i=0;i<memoria.size();i++){
            if(memoria[i]==-1){
                std::cout << "[-]";
            } else{
                std::cout << "[P" << memoria[i] <<"]";
            }
        }
        std::cout << "\n";
    }

    void mostrarTabela(){
        std::cout << "ID\tBase\tLimite\tTamanho\n:";
        if(tabela.empty()){
            std::cout <<"Tabela vazia!\n";
            return;
        }
        for(auto &entrada : tabela){
            const Processo &p=entrada.second;
            std::cout << "P" << p.id << "\t" << p.base << "\t" << p.limite << "\t" << p.tamanho << "\n";
        }
    }
}

int main(){ //main provisoria para testes
    // using namespace contigua;
    // inicMemoria(20);
    // mostrarMemoria();
    // mostrarTabela();
    return 0;
}

// rodar: cd GerenciamentoMemoria
// g++ contigua.cpp -o simulador
// ./simulador