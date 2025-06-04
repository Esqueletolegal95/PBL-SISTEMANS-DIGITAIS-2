# Biblioteca de comunicação entre o HPS e FPGA com o Coprocessador

O segundo problema da disciplina de MI - Sistemas Digitais tem como foco o desenvolvimento de uma biblioteca de software responsável por viabilizar a comunicação entre o HPS (Hard Processor System) e o FPGA (Field Programmable Gate Array) da placa DE1-SoC. Essa comunicação é realizada por meio do Platform Designer, utilizando interfaces PIO (Parallel Input/Output) mapeadas no barramento AXI.

A configuração dos PIOs permite o envio de instruções ao coprocessador, bem como a leitura de dados resultantes das operações executadas. Dessa forma, o objetivo principal é fornecer uma interface de software que abstraia os detalhes de hardware e permita que programadores de alto nível interajam com o coprocessador, integrando-o a aplicações escritas em linguagem C.

Link do coprocessador: https://github.com/DestinyWolf/CoProcessador_PBL2_SD_2025-1
## Plataform Designer e PIOs

Foram feitos 4 PIO's no Platform Designer:
- Inputs
  - Data_out: 8 bits
  - Flags (Done, overflow e incorrect address): 3 bits
- Outputs
  - Instruction: 18 bits
  - WR: 1 bit


## Biblioteca Assembly
A biblioteca oferencce as seguintes funções, onde são feitas as formatações das instruções com shift:
- Aritméticas:
  - Soma
  - Subtração
  - Multiplicação
  - Multiplicação por escalar
  - Reset

- Leitura/Escrita: 
  - Leitura
  - Escrita


## Testes
Foram realizados os seguintes testes pelo programa em C
- Escrita de matriz
- Leitura de matriz
- Multiplicação escalar com e sem overflow
- Adição de matrizes
- Subtração de matrizes
- Reset de matrizes



## Referências

- DE1-SoC User Manual - Terasic
- ARM Architecture Reference Manual - ARMv7
- Quartus Prime Lite 23.1 Documentation
- Github do Coprocessador: https://github.com/DestinyWolf/CoProcessador_PBL2_SD_2025-1
