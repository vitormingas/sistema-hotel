// ILRV-Hotel-Ativ04
// ILRV - 27/05/2025

// Sistema de reservas e check-in's de um hotel

// Inara Regina de Souza e Silva
// Liam Vedovato Lopes
// Rodrigo Ward Leite
// Vitor Gabriel de Sales Nicolau

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANDARES 20
#define APARTAMENTOS 14
#define TOTAL_APTS (ANDARES * APARTAMENTOS)
#define DISPONIVEL '*'
#define RESERVADO 'R'
#define OCUPADO 'O'

// Types
typedef struct endereco {
    char residencia[40];
    char munic[20];
    char UF[3];
    char cep[10];
    char pais[20];
} tipoEndereco;

typedef struct hospede {
    char cpf[30]; //aumentamos do tamanho 14 porque pode ser utilizado para o passaporte
    char nome[40];
    char telefone[15];
    int idade;
    tipoEndereco endereco;
    float valorHospedagem;
} tipoHospede;

typedef struct apartamento {
    char situacao; // O = Ocupado   R = Reservado * = Disponivel
    tipoHospede hospede;
} tipoApto;

// Prototipos

// Matriz
void exibirMatriz(tipoApto matriz[ANDARES][APARTAMENTOS]);
void resetMatriz(tipoApto matriz[ANDARES][APARTAMENTOS]);
void exibirTaxas(tipoApto matriz[ANDARES][APARTAMENTOS]);
// Reservas, check-in, check-out
void reservarApto(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
void cancelarReserva(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
void fazerCheckIn(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
void fazerCheckout(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
// Informacoes do hospede/apt
void cadastrarHospede(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
void exibirInfo(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt);
// Extras
int posicaoValida(int andar, int apt);
void fClear();
void fLimparTela();

// Main
int main() {

    int digito;
    tipoApto mapa[ANDARES][APARTAMENTOS];
    int andar = 0, apt = 0;
    
    // Iniciar a matriz com valores padrao
    resetMatriz(mapa);

    // Menu do usuario
    do {

        fLimparTela();
        exibirMatriz(mapa);    

        digito = 0;
        printf("< Painel de controle - Hotel ILRV >\n");
        printf("1. Check-in de hospede\n");
        printf("2. Check-out de hospede\n");
        printf("3. Reservar apartamento\n");
        printf("4. Cancelar uma reserva\n");
        printf("5. Informacoes do apartamento\n");
        printf("0. Sair do programa\n");

        printf("> ");
        scanf("%d", &digito);
        fClear();

        // Funcoes do menu
        switch (digito) {
            case 1:
                fazerCheckIn(mapa, andar, apt);
                break;
            case 2:
                printf("\nEntre com o andar e o apartamento desejado para fazer Check-Out (andar apt):\n> ");
                scanf("%d %d", &andar, &apt);
                fClear();
                fazerCheckout(mapa, andar, apt);
                break;
            case 3:
                printf("\nEntre com o andar e o apartamento desejado para reservar (andar apt):\n> ");
                scanf("%d %d", &andar, &apt);
                fClear();
                reservarApto(mapa, andar, apt);
                break;
            case 4:
                printf("\nEntre com o andar e o apartamento desejado para cancelar uma reserva (andar apt): \n> ");
                scanf("%d %d", &andar, &apt);
                fClear();
                cancelarReserva(mapa, andar, apt);
                break;
            case 5:
                printf("\nEntre com o andar e o apartamento que deseja visualizar (andar apt): \n> ");
                scanf("%d %d", &andar, &apt);
                fClear();
                exibirInfo(mapa, andar, apt);
                break;
            case 0:
                printf("\n> S a i n d o <\n");
                break;
            default:
                printf("\nOpcao invalida.\n\n");
                break;
        }

    } while (digito != 0);

    return 0;
}

// Funcoes

/* 
 ------------
| Utilidades |
 ------------
*/

// > Limpar buffer teclado
void fClear() {
	char carac;
	while ( (carac = fgetc(stdin) ) != EOF && carac != '\n') {
	}
}

// > Limpar a tela
void fLimparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// > Criacao e atribuicao de valores padrao da matriz
void resetMatriz(tipoApto matriz[ANDARES][APARTAMENTOS]) {
	int linha, coluna;
    for (linha = 0; linha < ANDARES; linha++) {
        for (coluna = 0; coluna < APARTAMENTOS; coluna++) {
            matriz[linha][coluna].situacao = DISPONIVEL;
        }
    }	
}

int posicaoValida(int andar, int apt) {
    return andar >= 1 && andar <= ANDARES && apt >= 1 && apt <= APARTAMENTOS;
}

/* 
 ---------
|Features |
 --------
*/

// > Exibir mapa do hotel
void exibirMatriz(tipoApto matriz[ANDARES][APARTAMENTOS]) {
    int andar = ANDARES;
    int i, linha, coluna;
    int disponivel = 0, reservado = 0, ocupado = 0;
    float porcentDisp = 0, porcentRes = 0, porcentOcup = 0;

    for (linha = 0; linha < ANDARES; linha++) {
        for (coluna = 0; coluna < APARTAMENTOS; coluna++) {
            switch (matriz[linha][coluna].situacao) {
                case DISPONIVEL: disponivel++; break;
                case RESERVADO: reservado++; break;
                case OCUPADO: ocupado++; break;
            }
        }
    }
    // 100.0f para forcar a funcao ficar em float
    porcentDisp = (100.0f * disponivel) / TOTAL_APTS;  
    porcentRes = (100.0f * reservado) / TOTAL_APTS;
    porcentOcup = (100.0f * ocupado) / TOTAL_APTS;

    printf("x------------------------------------------------------------------------------------x\n");
    printf("| Apt -  >   | ");
    for (i = 1; i <= APARTAMENTOS; i++) {
        printf("%2d   ", i);
    }
    printf("|\n");
    printf("|------------|-----------------------------------------------------------------------|\n");

    for (linha = 0; linha < ANDARES; linha++) {
        printf("| Andar %2d   | ", andar);
        for (coluna = 0; coluna < APARTAMENTOS; coluna++) {
            printf("%2c   ", matriz[linha][coluna].situacao);
        }
        printf("|\n");
        andar--;
    }

    printf("x------------------------------------------------------------------------------------x\n");
    printf("   * - disponivel %.1f%%          R - reservado %.1f%%             O - ocupado %.1f%%\n\n", porcentDisp, porcentRes, porcentOcup);
}

// > Reservar Apartamento
void reservarApto(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt) {
    if (!posicaoValida(andar, apt)) {
        printf("\n[!] Erro - Andar ou apartamento invalido.\n\n");
    } else {
        if (matriz[ANDARES - andar][apt - 1].situacao == DISPONIVEL) {
            matriz[ANDARES - andar][apt - 1].situacao = RESERVADO;
            cadastrarHospede(matriz, andar, apt);
            printf("\nReserva realizada com sucesso!\n\n");
        } else if (matriz[ANDARES - andar][apt - 1].situacao == OCUPADO) {
            printf("\n[!] Erro - Apartamento ocupado.\n\n");
        } else {
            printf("\n[!] Erro - Apartamento ja reservado.\n\n");
        }
    }
    system("pause");
}

// > Cancelar reserva
void cancelarReserva(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt) {
    if (!posicaoValida(andar, apt)) {
        printf("\n[!] Erro - Andar ou apartamento invalido.\n\n");
    } else {
        if (matriz[ANDARES - andar][apt - 1].situacao == DISPONIVEL) {
            printf("\n[!] Erro - Nenhuma reserva encontrada.\n\n");
        } else if (matriz[ANDARES - andar][apt - 1].situacao == OCUPADO) {
            printf("\n[!] Erro - Apartamento ocupado.\n\n");
        } else {
            matriz[ANDARES - andar][apt - 1].situacao = DISPONIVEL;
            printf("\nReserva cancelada com sucesso!\n\n");
        }
    }
    system("pause");
}

// > Fazer check-in
void fazerCheckIn(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt) {

    int opcao = 0;
    printf("\nHospede sabe o apartamento e andar da reserva: ");
    printf("\n(1) Sim\n(2) Nao\n> ");
    scanf("%d", &opcao);
    fClear();

    switch (opcao) {

        case 1:

            printf("\nEntre com o andar e o apartamento desejado para fazer Check-In (andar apt): \n> ");
            scanf("%d %d", &andar, &apt);
            fClear();

            if (!posicaoValida(andar, apt)) {
                printf("\n[!] Erro - Andar ou apartamento invalido.\n\n");
            } else {
                tipoApto *apto = &matriz[ANDARES - andar][apt - 1];
                int input = 0;

                if (apto->situacao == DISPONIVEL) {
                    apto->situacao = OCUPADO;
                    cadastrarHospede(matriz, andar, apt);
                    printf("\nCheck-In realizado com sucesso!\n\n");

                } else if (apto->situacao == OCUPADO) {
                    printf("\n[!] Erro - Apartamento ja ocupado!\n\n");

                } else { 
                    printf("\n[!] Esse apartamento esta reservado para \nDocumento: %s \nNome: %s\n", apto->hospede.cpf, apto->hospede.nome);
                    printf("Essa pessoa esta fazendo o check-in?\n");

                    do {
                        printf("(1) Sim\n(2) Nao\n> ");
                        scanf("%d", &input);
                        fClear(); 
                        if (input == 1) {
                            apto->situacao = OCUPADO;
                            printf("\nCheck-In do(a) %s realizado com sucesso!\n\n", apto->hospede.nome);
                        } else if (input == 2) {
                            printf("\nInforme o cliente para escolher outro quarto!\n\n");
                        }
                    } while (input != 1 && input != 2);
                }
            }
            break;

        case 2:

            char vProcurado[30];
            int encontrado = 0;
            printf("\nEntre com o documento\n> ");
            scanf("%[^\n]", vProcurado);

            for (int linha = 0; linha < ANDARES; linha++) {
                for (int coluna = 0; coluna < APARTAMENTOS; coluna++) {

                    tipoHospede h = matriz[linha][coluna].hospede;

                    if (matriz[linha][coluna].situacao == RESERVADO &&
                        (strcmp(h.cpf, vProcurado)) == 0) {

                        printf("\nHospede encontrado!\n");
                        exibirInfo(matriz, ANDARES - linha, coluna + 1);


                        printf("\nDeseja realizar o check-in neste apartamento?\n(1) Sim\n(2) Nao\n> ");
                        int escolha;
                        scanf("%d", &escolha);
                        fClear();

                        if (escolha == 1) {
                            matriz[linha][coluna].situacao = OCUPADO;
                            printf("\nCheck-in realizado com sucesso!\n");
                        } else {
                            printf("\nOperacao cancelada.\n");
                        }

                        encontrado = 1;
                        break;
                    }
                }
                if (encontrado) break;
            }

            if (!encontrado) printf("\nNenhum hospede com esse documento encontrado.\n\n");

            break;
    }

    system("pause");
}

// > Fazer check-out
void fazerCheckout(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt) {
    if (!posicaoValida(andar, apt)) {
        printf("\n[!] Erro - Andar ou apartamento invalido.\n\n");
    } else {
        if (matriz[ANDARES - andar][apt - 1].situacao == DISPONIVEL) {
            printf("\n[!] Erro - Nenhum check-in encontrado.\n\n");
        } else if (matriz[ANDARES - andar][apt - 1].situacao == OCUPADO) {
            matriz[ANDARES - andar][apt - 1].situacao = DISPONIVEL;
            printf("\nCheck-out realizado com sucesso!\n\n");
        } else {
            printf("\n[!] Erro - Nenhum check-in encontrado.\n\n");
        }
    }
    system("pause");
}

// > Cadastrar Hospede
void cadastrarHospede(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt){
	
	// %[^\n] evita o uso do fclear, porque não sobrecarrega o buffer , faz a leitura até antes do \n
	printf("\n-> Cadastro do apartamento %d, andar %d <-\n", apt, andar);
    printf("Nome do hospede: ");
    scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.nome);
    
    printf("Idade: ");
    scanf("%d", &matriz[ANDARES - andar][apt - 1].hospede.idade);
    fClear();

    printf("Telefone: ");
    scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.telefone);

    printf("Endereco: ");
    scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.endereco.residencia);
   
    printf("Documento: ");
    scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.cpf);
    
    printf("Pais: ");
    scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.endereco.pais);
    
    if(strcmp(matriz[ANDARES - andar][apt - 1].hospede.endereco.pais, "Brasil") == 0 ||
	strcmp(matriz[ANDARES - andar][apt - 1].hospede.endereco.pais,"brasil") == 0) {
    	
    	printf("Municipio: ");
        scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.endereco.munic);
        
        printf("Estado (UF): ");
        scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.endereco.UF);  
		
		printf("CEP: ");
        scanf(" %[^\n]", matriz[ANDARES - andar][apt - 1].hospede.endereco.cep); 
	}
    
     
}
 
// > Exibir informacoes do apartamento
void exibirInfo(tipoApto matriz[ANDARES][APARTAMENTOS], int andar, int apt) {
    if (!posicaoValida(andar, apt)) {
        printf("\n[!] Erro - Andar ou apartamento invalido.\n\n");
    } else {
        tipoApto apto = matriz[ANDARES - andar][apt - 1];

        printf("\n==============================\n");
        printf("Informacoes do apartamento\n");
        printf("Andar: %d | Apartamento: %d\n", andar, apt);
        printf("------------------------------\n");

        if (apto.situacao == DISPONIVEL) {
            printf("> Situacao:\n| Disponivel\n");
            printf("\n> Nenhum hospede cadastrado.\n");
        } else {
            if (apto.situacao == OCUPADO) {
                printf("> Situacao:\n| Ocupado\n");
            } else if (apto.situacao == RESERVADO) {
                printf("> Situacao:\n| Reservado\n");
            }

            tipoHospede h = apto.hospede;

            printf("\n> Dados do Hospede:\n");
            printf("| Nome: %s\n", h.nome);
            printf("| CPF: %s\n", h.cpf);
            printf("| Idade: %d\n", h.idade);
            printf("| Telefone: %s\n", h.telefone);
            printf("| Endereco: %s\n", h.endereco.residencia);
            printf("| Pais: %s\n", h.endereco.pais);
            
		if (strlen(h.endereco.munic) > 0 || strlen(h.endereco.UF) > 0 || strlen(h.endereco.cep) > 0){
			
            printf("| Municipio: %s\n", h.endereco.munic);
            printf("| Estado (UF): %s\n", h.endereco.UF);
            printf("| CEP: %s\n", h.endereco.cep);
         }
            
        }

        printf("==============================\n\n");
    }

    system("pause");
}
