[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2020a) > Trabalhos

# T4: Simulador de Propagação de Vírus com OpenMP


## Introdução


Neste trabalho, vamos paralelizar um programa que realiza experimentos com um simulador de propagação de um vírus. O simulador utiliza um modelo muito simples de propagação, em que pessoas são expostas ao vírus quando se aproximam de pessoas infectadas. A exposição pode ou não infectar uma pessoa, dependendo de uma probabilidade de propagação. O simulador testa várias probabilidades, utilizando amostras de números aleatórios  (na verdade, pseudo-aleatórios) para decidir se pessoas expostas vão ou não se infectar, de acordo com cada probabilidade. Essa abordagem, caracterizada pelo emprego de números aleatórios para representar valores de variáveis numa simulação, é conhecida como ["método de Monte Carlo"](https://en.wikipedia.org/wiki/Monte_Carlo_method). 

Métodos de Monte Carlo podem se aplicar a uma grande variedade de problemas, sendo muito utilizados para calcular soluções aproximadas (não exatas) para problemas numéricos oriundos das mais diversas áreas de conhecimento. Esses métodos competem com outros que também se aplicam a modelagem e simulação, sendo que em muitos casos não há consenso sobre qual método é mais adequado. Em geral, as aplicações do método de Monte Carlo são computacionalmente intensivas, pois é necessário repetir experimentos com diversas amostras de números para se fazer alguma análise estatística. Por outro lado, esses métodos podem oferecer fiferentes oportunidades de paralelização. É isso que será explorado neste trabalho, por meio da paralelização do código original com OpenMP, para execução em uma arquitetura paralela **multicore**.


**IMPORTANTE**: O tema deste trabalho foi inspirado no contexto atual da pandemia do Covid-19, mas o programa não leva em consideração características específicas da propagação do coronavírus causador dessa doença. O modelo usado é demasiadamente simples e foi adaptado de um simulador criado para ilustrar a aplicação de um método de Monte Carlo.

## Desenvolvimento


1. Este trabalho pode ser feito **em dupla ou individualmente**.

2. Para começar, obtenha o arquivo [virusim.zip](virusim.zip), que contém o programa sequencial de referência para este trabalho. Obs.: como em muitas situações no mundo real, a documentação é o próprio código, então acostume-se a lidar com isso :-D

3. Teste o programa sequencial com difentes parâmetros de entrada. Analise o código e identifique oportunidades de paralelismo, considerando a arquitetura paralela alvo. 

4. Projete **duas soluções** paralelas para o programa em questão e implemente-as usando OpenMP. Você pode, por exemplo, variar as estratégias de particionamento, de escalonamento, etc.

5.  Avalie o desempenho dos programas medindo os tempos de execução sequencial e paralela para diferentes configurações do problema (mínimo 3: um problema grande, um médio e um pequeno), e com diferentes números de threads. Utilize a mesma técnica de medição de tempos para os dois programas. A partir dos tempos medidos, calcule aceleração e eficiência para cada caso.

6. Elabore **slides** explicando as estratégias de paralelização adotadas, descrevendo os experimentos realizados e discutindo os resultados obtidos. Na descrição dos experimentos, apresente a especificação de todo o material utilizado (hardware e software).


## Entrega

No repositório de entrega do trabalho, crie um documento `Entrega.md`, contendo:
 - Identificação completa da disciplina e do(s) aluno(s);
 - Links para os códigos desenvolvidos;
 - Link para os slides elaborados;
 - Referências.


