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
    void mostraMemoria();
    void mostraTabela();
    void processo_FirstFit(int id, int tamanho);
    void removerProcessoContigua(int id);
    void realocarMemoria();
    
}

#endif 