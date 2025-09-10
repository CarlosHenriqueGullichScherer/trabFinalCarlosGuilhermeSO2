#include "memoria.hpp"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <map>

namespace gerenMemoria {

    bool ehValido(const std::string &s){
        if(s.empty()) return true;

       if(s[0] == '-') return true;

        for(int i = 0; i < (int)s.size(); i++){
            if(!isdigit(s[i])) return true;
        }
        return false;
    }
  
    int pedirTamanhoPagina(){
        std::string tamanhoPagina;
        do{
            std::cout <<"Digite o tamanho da página em KB: ";     
            std::getline(std::cin, tamanhoPagina);
        } while (ehValido(tamanhoPagina));
        
        return std::stoi(tamanhoPagina);
    }

    int pedirNumeroFrames(){
        std::string nFrames;
        do{
            std::cout <<"Digite o número de frames: ";
            std::getline(std::cin, nFrames);
        } while (ehValido(nFrames));
        
          return std::stoi(nFrames);
    }

    void desenharFramesVazios(std::vector<std::string>& frames){
        for(auto& frame : frames){
            frame = "[ ]";
        }
    }

    void inserirProcesso(int &contador, int tamanhoDaPagina, std::vector<processo>& processos){
        std::string tamanhoDoProcesso;
        std::cout <<"Você está prestes a inserir um processo!" << std::endl;    

        do{
            std::cout << "Digite o tamanho total em KB que o processo oucupará: ";
            std::getline(std::cin, tamanhoDoProcesso);
        } while (ehValido(tamanhoDoProcesso));
        

        std::cout << "O processo que você inseriu terá o ID " << contador << std::endl;
        
        processo atual;
        atual.id = contador;
        atual.nPaginas = std::ceil((double)std::stoi(tamanhoDoProcesso) / tamanhoDaPagina);
        atual.tamanhoTotal = std::stoi(tamanhoDoProcesso);
        atual.fragmentacao = std::stoi(tamanhoDoProcesso) % tamanhoDaPagina;

        contador++;

        processos.push_back(atual);
    }

    void alocarProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos){
        int contador = processos.back().nPaginas;    
        for(auto& frame : frames){
                if(frame == "[ ]"){
                    frame = "[";
                    frame += std::to_string(id);
                    frame += "]";
                    contador--;
                    if(contador == 0) break;
                }
            }
    }

    void removerProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos){
        std::string alvo = "[";
        alvo += std::to_string(id);
        alvo += "]";
        for(auto & frame : frames){
            if(frame == alvo){
                frame = "[ ]";
            }
        }
    }

    void exibirTabelaPaginas(std::vector<std::string> const & frames, int id, std::vector<processo>& processos){
        int ID = processos.back().id;
        std::string alvo = "[";
        alvo += std::to_string(ID);
        alvo += "]";
        int contador = 0;
        for(size_t i = 0; i < frames.size(); i++){
            if(frames[i] == alvo){
                processos.back().tabelaDePaginas[contador] = i;
                contador++;
            }
        }
        std::cout << "Página    " << "Frame" << std::endl;
        for(auto &[chave, valor] : processos.back().tabelaDePaginas){
            std::cout <<  chave << "            " << valor << std::endl;
        }
    }

    void desenharFrames(std::vector<std::string>const & frames){
        int contador = 0;
        for(auto & frame: frames){
            std::cout << frame << " ";
            contador ++;
            if(contador >= 10){
                contador = 0;
                std::cout << std::endl;
            }
        }
    }


}   


int main(){
    int idPaginaAtual = 0;
    const int NFRAMES = gerenMemoria::pedirNumeroFrames();
    const int TPAGINA = gerenMemoria::pedirTamanhoPagina();
    std::vector<std::string> frames(NFRAMES);
    std::vector<gerenMemoria::processo> processos;
    gerenMemoria::desenharFramesVazios(frames);

    gerenMemoria::inserirProcesso(idPaginaAtual,TPAGINA, processos);
    gerenMemoria:: alocarProcesso(frames,processos.back().id,processos);
    gerenMemoria:: exibirTabelaPaginas(frames,processos.back().id,processos);
    gerenMemoria::inserirProcesso(idPaginaAtual,TPAGINA, processos);
    gerenMemoria:: alocarProcesso(frames,processos.back().id,processos);
    gerenMemoria:: exibirTabelaPaginas(frames,processos.back().id,processos);
    gerenMemoria::inserirProcesso(idPaginaAtual,TPAGINA, processos);
    gerenMemoria:: alocarProcesso(frames,processos.back().id,processos);
    gerenMemoria:: exibirTabelaPaginas(frames,processos.back().id,processos);

    gerenMemoria:: desenharFrames(frames);
    gerenMemoria:: removerProcesso(frames,processos[processos.size() - 2].id,processos);
    std::cout << std::endl;
    gerenMemoria:: desenharFrames(frames);
    gerenMemoria::inserirProcesso(idPaginaAtual,TPAGINA, processos);
    gerenMemoria:: alocarProcesso(frames, processos.back().id,processos);
    gerenMemoria:: exibirTabelaPaginas(frames,processos.back().id,processos);
    gerenMemoria:: desenharFrames(frames);

}
