# Aventuras Embarcadas - Aprendendo Brincando - Aprendizado interativo utilizando técnicas de recordação ativa 
![IMG_2987 (1)](https://github.com/user-attachments/assets/0896dbe0-79aa-4241-88e2-2344559fa2a9)
![IMG_2984 (1)](https://github.com/user-attachments/assets/1405652e-9eb8-4c9a-9848-4a8528da75c7)


# Introdução 
- O projeto apresentado neste relatório busca utilizar um sistema embarcado para auxiliar na retenção de informações e aprendizado de estudantes por meio da placa educacional BitDogLab. 
Agindo como um quiz ou até mesmo um flashcard interativo, esse sistema deve funcionar apresentando perguntas, que podem ser puladas, e corrigindo se o usuário acertou ou errou. Funcionando como uma maneira digital de aplicar os princípios da recordação ativa, estimulando o aprendizado de maneira interativa.  
# Objetivos 
- O sistema embarcado deve ser capaz de reproduzir questões, analisar e corrigir as respostas e produzir uma saída interativa de LED e display que indiquem se a resposta foi respondida corretamente ou não e contar o número de acertos.
O projeto deve ser capaz de auxiliar no aprendizado de conteúdos especificados no software, aplicando os princípios da recordação ativa e repetição para reforçar o entendimento e incentivar a formação da memória de longo prazo em relação ao aprendizado.
# Requisitos 
- O exemplo de usuário usado para este projeto é de um estudante do Ensino Fundamental I, uma fase de extrema importância para o desenvolvimento acadêmico, nessa idade um dos desafios para este estudante seria as tabelas de multiplicação e também divisões simples. 
O produto deve apresentar questões de matemática do nível de aprendizado deste aluno. 
Requisitos funcionais - Um grande desafio para alunos do primeiro nível do Ensino Fundamental é a memorização da tabuada e compreensão de multiplicações e divisões simples. Então, o sistema deve apresentar questões de matemática de maneira interativa e atrativa, contabilizando o número de acertos, incentivando o aluno a ver esse momento de estudo como uma brincadeira. 
Requisitos não-funcionais- Deve ter uma performance rápida, ter uma bateria própria com bom consumo de energia e mantendo em mente que o usuário é uma criança, deve ter uma interface de fácil uso. 
# Descrição do funcionamento 
- O projeto deve funcionar como um jogo interativo de perguntas e respostas educacionais. O display OLED mostrará as questões, as respostas e quantas questões o usuário acertou, as questões devem ser respondidas escolhendo entre as opções, pressionando os botões A (esquerdo) ou B(direito), se o usuário deseja pular uma questão basta indicar para baixo com o joystick; se deseja voltar para o início e resetar a contagem, basta pressionar o joystick; e se deseja voltar uma questão, basta indicar para direita. 
Se o usuário responder corretamente, tanto o display indicará, mostrando “correta”, quanto a matriz de LED brilhará em uma padrão azul, adicionando +1 à contagem de acertos; caso a resposta esteja incorreta, o display indicará “incorreta” e a matriz brilhará em uma padrão vermelho.
# Lista de materiais 
- 1 Microprocessador Raspberry Pi Pico W;
- 1 Microcontrolador RP2040;
- 1 Display OLED 128x64 0.96’’;
- 1 Matriz módulo 5x5 LEDs RGB ws2812b 5050;
- 2 Botões push button 6x6x5 mm;
- 1 Joystick;
- 1 Bateria Li-ion 18.650 3.600 mAh 3,7V recarregável;
- 1 Case de bateria 18650.
# Hardware 
- Microprocessador Raspberry Pi Pico W:  Processa as instruções de programas, realiza cálculos e operações lógicas;
- Microcontrolador RP2040: circuito integrado que controla o sistema;
- Matriz de LED: Após a questão ser respondida por meio dos botões a matriz, por meio de cores, azul e vermelho, demonstra se a resposta do usuário está correta (azul) ou incorreta (vermelha). Inicialização PIO e configuração da WS2812; 
- Display OLED: Exibe as questões, indicam o usuário as opções de resposta, opção A ou opção B, indica se a resposta está correta ou incorreta e também conta quantas perguntas foram respondidas corretamente. Inicialização I2C e inicialização OLED;
- Botão A e botão B: Onde o usuário escolhe a opção desejada de resposta, o botão recebe a resposta e a envia ao microprocessador para ser processada e exibida. Inicialização GPIO e resistores pull up;
- Joystick analógico: Onde o usuário pode pular uma questão indesejada, indicando para baixo, voltar para o início pressionando o joystick ou voltar uma questão indicando para direita. Funciona como os botões. Inicialização GPIO e resistores pull up;
- Bateria recarregável: Para que o usuário possa usar o produto por períodos de tempo sem necessitar uma conexão constante com um interruptor. 
