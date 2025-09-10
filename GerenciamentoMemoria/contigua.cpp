#include <iostream>
#include <vector>
#include <map>
#include "contigua.hpp"

namespace contigua{
    //variaveis internas
    static std::vector<int> memoria; //static para evitar conflito e garantir usabilidade em todo o programa - vector represntando celulas da memoria
    static std::map<int, Processo> tabela; //map representa tabela de processos
    static int ultimoId=1; //contador de IDs de processo

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
        std::cout << "ID\tBase\tLimite\tTamanho\n";
        if(tabela.empty()){
            std::cout <<"Tabela vazia!\n";
            return;
        }
        for(auto &entrada : tabela){
            const Processo &p=entrada.second;
            std::cout << "P" << p.id << "\t" << p.base << "\t" << p.limite << "\t" << p.tamanho << "\n";
        }
    }

    void processo_FirstFit(int id, int tamanho){
        int livre=0; //contador de posicoes livres
        int inicio=-1; //posicao onde comeca o bloco livre
        for(int i=0;i<(int)memoria.size();i++){
            if(memoria[i]==-1){
                if(inicio==-1){
                    inicio=i;
                }
                livre++;
                
                if(livre==tamanho){
                    Processo p;
                    p.id=ultimoId++;
                    p.base=inicio;
                    p.limite=inicio+tamanho-1;
                    p.tamanho=tamanho;

                    tabela[p.id]=p;

                    for(int j=inicio;j<=p.limite;j++){
                        memoria[j]=p.id;
                    }

                    std::cout << "Processo P" << p.id << " criado. (Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
                    return;
                }
            } else{
                inicio=-1;
                livre=0;
            }
        }
        std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
    }

    void removerProcessoContigua(int id){
        auto it=tabela.find(id);
        if(it==tabela.end()){
            std::cout << "Processo P" << id << " nao encontrado!\n";
            return;
        }

        Processo p = it->second;

        for(int i=p.base;i<=p.limite;i++){
            memoria[i]=-1;
        }

        tabela.erase(it);

        std::cout << "Processo P" << id << " removido com sucesso!\n";
    }

    void realocarMemoria(){
        int posLivre=0;

        for(auto &kv : tabela){
            Processo &p=kv.second;
            if(p.base != posLivre) {
            for (int i=0;i<p.tamanho;i++) {
                memoria[posLivre+i]=p.id;
                memoria[p.base+i]=-1;
            }
            p.base=posLivre;
            p.limite=posLivre+p.tamanho-1;
            }
            posLivre = p.limite + 1;
        }
        std::cout << "Memoria realocada!\n";
    }
}


int main(){ //main provisoria para testes
    using namespace contigua;
    inicMemoria(20);
    processo_FirstFit(1, 5);  // cria processo 1 com tamanho 5
    processo_FirstFit(2, 7);  // cria processo 2 com tamanho 7
    processo_FirstFit(3, 4);
    mostrarMemoria();
    mostrarTabela();
    removerProcessoContigua(2);
    mostrarMemoria();
    mostrarTabela();
    realocarMemoria();
    mostrarMemoria();
    mostrarTabela();
    return 0;
}

// rodar: cd GerenciamentoMemoria
// g++ contigua.cpp -o simulador
// ./simulador