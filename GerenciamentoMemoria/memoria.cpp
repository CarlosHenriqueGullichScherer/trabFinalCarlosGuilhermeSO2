#include "memoria.hpp"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <map>

namespace gerenMemoria {

    bool ehValido(const std::string &s){ //função feita para validar entradas em loops, nome ehInvalido faria mais sentido
        if(s.empty()) return true; //só aceita números inteiros maiores que 0

       if(s[0] == '-') return true;

       if(s[0] == '0') return true;

        for(int i = 0; i < (int)s.size(); i++){
            if(!isdigit(s[i])) return true;
        }
        return false;
    }
  
    int pedirTamanhoPagina(){ //valida e pede tamanho da página
        std::string tamanhoPagina;
        do{
            std::cout <<"Digite o tamanho da página em KB: ";     
            std::getline(std::cin, tamanhoPagina);
        } while (ehValido(tamanhoPagina));  //não para de pedir até a entrada ser válida
        
        return std::stoi(tamanhoPagina);
    }

    int pedirNumeroFrames(){ //valida e pede número de frames
        std::string nFrames;
        do{
            std::cout <<"Digite o número de frames: ";
            std::getline(std::cin, nFrames);
        } while (ehValido(nFrames));
        
          return std::stoi(nFrames);
    }

    void desenharFramesVazios(std::vector<std::string>& frames){
        for(auto& frame : frames){
            frame = "[ ]"; //desenha a memória com cada frame sendo um conchete, salvo em  um vetor de strings
        }
    }

    void inserirProcesso(int &contador, int tamanhoDaPagina,  std::vector<std::string> frames, std::vector<processo>& processos){
        std::string tamanhoDoProcesso;
        std::cout <<"Você está prestes a inserir um processo!" << std::endl;    
        int totalMemoriaLivre;
        int frameslivres = 0;
        for(size_t i = 0; i < frames.size(); i++){
            if(frames[i] == "[ ]")
                frameslivres++; //descobre quantos frames livres a memória tem
        }
        if(frameslivres == 0){
            std::cout << "Você não pode inserir nada!, não há frames livres " << std::endl;
            return; //se não há frames livres, não se pode inserir nada
        }
        totalMemoriaLivre = tamanhoDaPagina * frameslivres; // calcula o total de memória livre para permitir inserir processos ou não
        do{ // se mantém nesse loop até a entrada ser válida (int) e ao mesmo tempo ser um valor que cabe na memória
        std::cout <<"Digite o tamanho da página em KB: ";
        std::getline(std::cin, tamanhoDoProcesso);
        while (ehValido(tamanhoDoProcesso)){
            std::cout << "Digite uma entrada válida: ";
            std::getline(std::cin, tamanhoDoProcesso);
        }
        
        
            if (std::stoi(tamanhoDoProcesso) > totalMemoriaLivre){
                std::cout << "A memória não suporta um processo desse tamanho no momento" << std::endl;
            }
            
        } while (std::stoi(tamanhoDoProcesso) > totalMemoriaLivre || ehValido(tamanhoDoProcesso));
        

        std::cout << "O processo que você inseriu terá o ID " << contador << std::endl; //todo processo inserido recebe o id 1, e é incrementado,
        //usuário apenas escolhe o tamanho total do processo
        
        processo atual; //a partir do tamanho do processo já alimenta a struct do processo com o que pode ser deduzido
        atual.id = contador;
        atual.nPaginas = std::ceil((double)std::stoi(tamanhoDoProcesso) / tamanhoDaPagina);
        atual.tamanhoTotal = std::stoi(tamanhoDoProcesso);
        int ultimaPag = std::stoi(tamanhoDoProcesso) % tamanhoDaPagina;
        if(ultimaPag){
            atual.fragmentacao = tamanhoDaPagina - ultimaPag;
        }else{
            atual.fragmentacao = 0;
        }
        contador++;

        processos.push_back(atual); //coloca o processo no vector dos processos em memória
    }

    void alocarProcesso(std::vector<std::string>& frames, int id, std::vector<processo>& processos){
        int contador = processos.back().nPaginas;    //sempre é chamada pelo programa após a função inserir processo, por isso do back()
        for(auto& frame : frames){ //procura frames vazios e desenha o id do processo neles [ ] -- > [1]
                if(frame == "[ ]"){ 
                    frame = "[";
                    frame += std::to_string(id);
                    frame += "]";
                    contador--;
                    if(contador == 0) break; //para quando colocar o número de páginas que o processo precisa
                }
            }
    }

    void removerProcesso(std::vector<std::string>& frames, int idProcurado, std::vector<processo>& processos){
        int posicao = 0;
        std::string alvo = "[";
        alvo += std::to_string(idProcurado);
        alvo += "]";
        for(auto & frame : frames){ //desenha os frames que o processo estava ocupando como frames livres
            if(frame == alvo){
                frame = "[ ]";
            }
        }
        for(processo* p = processos.data(); p < processos.data() + processos.size(); p++){ //retira o processo do vetor de processos em memória
            if(p->id == idProcurado){
                processos.erase(processos.begin() + posicao); 
            }
            posicao++;
        }
    }

    void exibirTabelaPaginas(std::vector<std::string> const & frames, int id, std::vector<processo>& processos){
        std::string alvo = "[";
        alvo += std::to_string(id);
        alvo += "]";
        int contador = 0;
        for(size_t i = 0; i < frames.size(); i++){ //percorre a memória e descobre se os frames que o processo está ocupando
            if(frames[i] == alvo){ //se o frame contém o id alvo
                
                //estou sempre armazenando o tabela de páginas num último processo inserido, isso a princípio não faz sentido,
                //porém não altera o funcionamento da didática do programa, já que a tabela é sempre mapeada e exibida
                //mais fácil de implementar
                
                processos.back().tabelaDePaginas[contador] = i; //contador (página) corresponde ao indice do vetor (frame)
                contador++;
            }
        }
        std::cout << "Página    " << "Frame" << std::endl; //exibe a página chave e o frame valor correspondentes
        for(auto &par: processos.back().tabelaDePaginas){
            auto chave = par.first;
            auto valor = par.second;
            std::cout <<  chave << "            " << valor << std::endl;
        }
        processos.back().tabelaDePaginas.clear(); //limpa a tabela para próxima busca não ter colisão, já que eu sempre armazeno no último
    }

    void desenharFrames(std::vector<std::string>const & frames){ //simplesmente exibe a memória como ela está, exatamente com os processos alocados e a quais frames eles estão ocupando
        int contador = 0; 
        for(size_t i = 0; i < frames.size(); i++){
            std::cout << frames[i] << " ";
            contador ++;
            if(contador >= 10){ // a cada dez frames gera uma nova linha para melhor vizualização
                contador = 0;
                std::cout << std::endl;
            }
        }
    }

    double calcularFragmentacao(std::vector<processo>const & processos, int nframes, int tpagina){
        double fragmentacao = 0;
        for(auto p : processos){
            fragmentacao += p.fragmentacao; //acumula os bits não utilizados de todos os processos
        }
            return  (fragmentacao / (double)(nframes * tpagina)) * 100;   //calcula a fragmentação total em porcentagem

    }

    std::string calcularFragmentacaoEspeci(std::vector<processo>& processos, int tpagina, int idProcurado){
        processo processoLocal;
        double fragTotal;
        double fragPagina;
        for (auto processo : processos){
            if (processo.id == idProcurado){
                processoLocal = processo; //descobre o processo que o usuário quer ver a fragmentação a partir do id passado
            }
        }
        fragTotal = (double) processoLocal.fragmentacao / processoLocal.tamanhoTotal * 100; //calcula a fragmentação em relação ao processo
        fragPagina = (double) processoLocal.fragmentacao / tpagina * 100; // em relação a última página
        return "Relativa ao processo: " + std::to_string(fragTotal) + "% " "\n" + "Relativa a última pag " + std::to_string(fragPagina) + "%";
       
    }

    int acharProcesso(std::vector<processo>&processos){ //na verdade acha o id do processo, as outras funções acabam achando a struct completa se precisarem como na função acima
        std::string id;
        bool entradaInvalida = true;
        do{ //mantém em loop até o usuário digitar um Int e ao mesmo tempo um dos ids disponíveis
        std::cout << "Qual processo você quer selecionar(ID): " << std::endl;
        std::string ids;
        for(auto& processo : processos){
            ids += std::to_string(processo.id) + " ";
        }
        std::cout << "Ids: disponíveis: " << ids << std::endl;
        std::getline(std::cin, id);
        ids = "";
        if(ehValido(id)) continue;
        for(auto& processo : processos){
            if(processo.id == std::stoi(id)){
                entradaInvalida = false;
                break;
            }
        }
        }while (ehValido(id) || entradaInvalida);
        
       for (auto& processo : processos) { //retorna então o processo que o usuário digitou, percorrendo o vetor para ter certeza que existe
        if (processo.id == std::stoi(id)) {
            return processo.id;
        }
    }
        return 0; //retorna zero caso não ache nada, não achei um caso que isso acontecesse
    }

    void menuPaginacao() { //mantém o usuário no menu até que ele queria sair
    bool sair = false;

    while (!sair) {  
        std::cout << "Você está no menu da paginação, para começar uma simulação, "
                  << "você deve inserir a quantidade de frames da memória e tamanho da página" 
                  << std::endl; //pede essas informações toda vez que resetar o menu

        int tPagina = pedirTamanhoPagina();
        int nFrames = pedirNumeroFrames();

        std::vector<std::string> frames(nFrames); //representa a memória
        desenharFramesVazios(frames); // precisa da memória desenhada vazia como [ ] em cada frame para funcionar
        std::vector<processo> processos; //vetor para os processos em memória, guardar os dados deles
        int contador = 1; //processo começa com o id e próximo com id 2 e assim por diante

        bool reiniciar = false;

        while (!reiniciar && !sair) {
            std::string entrada;
            std::cout << "\nSelecione uma das opções abaixo: " << std::endl; 
            std::cout << "1. Inserir Processo " << std::endl;
            std::cout << "2. Remover Processo " << std::endl;
            std::cout << "3. Exibir tabela de páginas " << std::endl;
            std::cout << "4. Ver Memória " << std::endl;
            std::cout << "5. Calcular Fragmentação " << std::endl;
            std::cout << "6. Calcular Fragmentação de um processo específico " << std::endl;
            std::cout << "7. Reiniciar simulação" << std::endl;
            std::cout << "8. Sair " << std::endl;

            std::getline(std::cin, entrada);

            if (ehValido(entrada)) continue;
            int opcao = std::stoi(entrada);

            switch (opcao) {
                case 1: {
                    inserirProcesso(contador, tPagina, frames, processos);
                    alocarProcesso(frames, processos.back().id, processos);
                    break;
                }
                case 2:
                    if (!processos.empty())
                        removerProcesso(frames, acharProcesso(processos), processos);
                    else 
                        std::cout << "Não há nenhum processo" << std::endl;
                    break;
                case 3:
                    if (!processos.empty())
                        exibirTabelaPaginas(frames, acharProcesso(processos), processos);
                    else
                        std::cout << "Não há nenhum processo" << std::endl;
                    break;
                case 4:
                    desenharFrames(frames);
                    break;
                case 5: {
                    if(!processos.empty()){
                    double resultado = calcularFragmentacao(processos, nFrames, tPagina);
                    std::cout << resultado << "%" << std::endl;
                    }else
                        std::cout << "Não há nenhum processo" << std::endl;
                    break;
                }
                case 6:
                    if (!processos.empty()){
                     std::cout << calcularFragmentacaoEspeci(processos, tPagina, acharProcesso(processos));
                    }else
                        std::cout << "Não há nenhum processo" << std::endl;
                    break;
                case 7:
                    reiniciar = true; // volta para pedir tPagina e nFrames
                    break;
                case 8:
                    sair = true; // sai do programa
                    break;
                default:
                    std::cout << "Opção inválida!" << std::endl; //nunca cai no default, está por padrão
                    break;
            }
        }
    }
}

}



