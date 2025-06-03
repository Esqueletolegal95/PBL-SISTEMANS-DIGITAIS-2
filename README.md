# Biblioteca de comunicação entre o HPS e FPGA com o Coprocessador

O segundo problema da disciplina de MI - Sistemas Digitais tem como foco o desenvolvimento de uma biblioteca de software responsável por viabilizar a comunicação entre o HPS (Hard Processor System) e o FPGA (Field Programmable Gate Array) da placa DE1-SoC. Essa comunicação é realizada por meio do Platform Designer, utilizando interfaces PIO (Parallel Input/Output) mapeadas no barramento AXI.

A configuração dos PIOs permite o envio de instruções ao coprocessador, bem como a leitura de dados resultantes das operações executadas. Dessa forma, o objetivo principal é fornecer uma interface de software que abstraia os detalhes de hardware e permita que programadores de alto nível interajam com o coprocessador, integrando-o a aplicações escritas em linguagem C.

Link do coprocessador: https://github.com/DestinyWolf/CoProcessador_PBL2_SD_2025-1
## Plataform Designer e PIOs

Foram feitos 4 PIO's no Platform Designer:
-Inputs
  -Data_out: 8 bits
  -Flags: 3 bits
-Outputs
  -Instruction: 18 bits
  -WR: 1 bit


## 🛠️ Funcionalidades

- [x] Funcionalidade legal 1
- [x] Funcionalidade fofa 2
- [ ] Coisas que ainda vai fazer

## 🚀 Como usar

### Pré-requisitos

- Algo que precisa instalar (`npm`, `python`, `gcc`, etc.)
- Outro requisito

### Instalação

```bash
# Clone este repositório
git clone https://github.com/usuario/repositorio

# Acesse a pasta do projeto
cd repositorio

# Instale as dependências (se for o caso)
npm install
