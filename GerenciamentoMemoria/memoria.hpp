#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <map>
#include <string>
#include <vector>

namespace gerenMemoria{


struct processo{
    int nPaginas;
    int framesNecessarios;
    int id;
    int fragmentacao;
    double tamanhoTotal;
    std::map<int, int> tabelaDePaginas;

};
bool ehValido(const std::string &s);
int pedirTamanhoPagina();
int pedirNumeroFrames();
void desenharFramesVazios(std::vector<std::string>& frames);
processo inserirProcesso(int &contador, int tamanhoDaPagina);

void resetar();
void sair();
void modoDeOperação();
void calcularFragmentação();
void espacoLivre();
void fragmentosDisponiveis();
void fragmentacaoInterna();
void fragmentacaoExterna();
void taxaSucessoAlocacao();
void framesLivres();
void usoMemoria();
void menuInterativo();

}

#endif 