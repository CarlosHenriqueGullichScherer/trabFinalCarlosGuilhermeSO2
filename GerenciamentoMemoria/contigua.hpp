#ifndef CONTIGUA_HPP
#define CONTIGUA_HPP

#include <map> //guardar a tabela de processos

namespace contigua{

    struct Processo{
        int id;
        int tamanho;
        int base;
        int limite;
    };

    void inicMemoria(int n);
    void mostrarMemoria();
    void mostrarTabela();
    void processo_FirstFit(int tamanho);
    void processo_BestFit(int tamanho);
    void processo_WorstFit(int tamanho);
    void processo_CircularFit(int tamanho);
    void removerProcessoContigua(int id);
    void realocarMemoria();
    double calcularFragmentacao();
    
}

#endif 