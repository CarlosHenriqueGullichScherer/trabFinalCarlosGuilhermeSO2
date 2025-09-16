#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <map>
#include <string>
#include <vector>
//namespace que implementa paginação
namespace gerenMemoria{


struct processo{ //struct para representar dados relevantes ao processo
    int nPaginas;
    int id;
    int fragmentacao;
    double tamanhoTotal;
    std::map<int, int> tabelaDePaginas;

};
bool ehValido(const std::string &s); //valida entradas
int pedirTamanhoPagina(); 
int pedirNumeroFrames();
void desenharFramesVazios(std::vector<std::string>& frames); //desenha a tabela vazia para ser preeenchida futuramente
void inserirProcesso(int &contador, int tamanhoDaPagina,  std::vector<std::string> frames, std::vector<processo>& processos); //insere e valida processo num vetor
void alocarProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos); //desenha o processo na memória
void removerProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos); //tira o processo do vetor e da memória
void desenharFrames(std::vector<std::string>const & frames); //exibe o estado atual da memória
void exibirTabelaPaginas(std::vector<std::string> const & frames, int id, std::vector<processo>& processos); //mapeia e exibe a tabela de páginas
double calcularFragmentacao(std::vector<processo>const & processos); //fragmentação ao todo
std::string calcularFragmentacaoEspeci(std::vector<processo>& processos, int tpagina, int idProcurado); //fragmentação de um processo
int acharProcesso(std::vector<processo>&processos); //função auxiliar
void menuPaginacao(); 
}

#endif 