#include "memoria.hpp"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>

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

    processo inserirProcesso(int &contador, int tamanhoDaPagina){
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

        return atual;
    }
}   


int main(){
    int idPaginaAtual = 0;
    const int NFRAMES = gerenMemoria::pedirNumeroFrames();
    const int TPAGINA = gerenMemoria::pedirTamanhoPagina();
    std::vector<std::string> frames(NFRAMES);
    gerenMemoria::desenharFramesVazios(frames);

    gerenMemoria::processo processoLocal = gerenMemoria::inserirProcesso(idPaginaAtual,TPAGINA);
    std::cout << "id " << processoLocal.id << " n de páginas " << processoLocal.nPaginas << " tamanho total" << processoLocal.tamanhoTotal << " fragmentação" << processoLocal.fragmentacao;

}
