#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <map>
#include <string>
#include <vector>

namespace gerenMemoria{


struct processo{
    int nPaginas;
    int id;
    int fragmentacao;
    double tamanhoTotal;
    std::map<int, int> tabelaDePaginas;

};
bool ehValido(const std::string &s);
int pedirTamanhoPagina();
int pedirNumeroFrames();
void desenharFramesVazios(std::vector<std::string>& frames);
void inserirProcesso(int &contador, int tamanhoDaPagina, std::vector<processo>& processos);
void alocarProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos);
void removerProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos);
void desenharFrames(std::vector<std::string>const & frames);
void exibirTabelaPaginas(std::vector<std::string> const & frames, int id, std::vector<processo>& processos);
}

#endif 