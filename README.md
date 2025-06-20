Máscara Automática de Componentes em Dados Meteorológicos

Este programa em C identifica componentes conexos com mesmo sinal (positivo ou negativo) em campos meteorológicos bidimensionais, armazenados em formato binário. Os componentes são definidos como conjuntos de células adjacentes com mesmo sinal (valor positivo ou negativo), formando regiões conectadas.

O dado é tratado como um grafo não direcionado, onde cada célula é um vértice com até 8 vizinhos (adjacência de Moore). O programa gera automaticamente máscaras binárias (em novos arquivos binários) para os componentes com tamanho superior a um limiar mínimo.

📂 Entrada
    
    Um arquivo binário contendo nx * ny floats, representando um campo bidimensional.
    Parâmetros de dimensão: nx (largura), ny (altura), nt (tempo, atualmente não utilizado no loop principal).
    Valores indefinidos devem estar marcados com 777.7 (modificável no código).
    O programa ignora células indefinidas ao buscar componentes.

🧮 Saída
    Um arquivo binário com até MAX_OUT_NT (padrão: 30) campos bidimensionais (nx × ny).
    Cada campo representa uma máscara com valores:
        1.0 para células pertencentes ao componente detectado.
        777.7 para demais posições (valor indefinido).
    Cada máscara representa um componente de tamanho ≥ MIN_COMPONENT_SIZE (padrão: 80).

▶️ Uso
```
./mascara_componentes <arquivo_entrada> <nx> <ny> <nt> <arquivo_saida>
```
Exemplo:
```
./mascara_componentes dados.bin 100 80 1 saida.bin
```

🧠 Como Funciona

    Leitura dos dados binários: O programa lê nx * ny valores float de um arquivo.
    Busca por componentes conexos:
        Para cada célula ainda não visitada:
            Executa uma BFS para encontrar todas as células conectadas com o mesmo sinal (positivo ou negativo).
            Ignora células com valor indefinido.
            Marca todas as células do componente com valor 1.0 na máscara de saída.
            Salva a máscara se o componente tiver tamanho ≥ MIN_COMPONENT_SIZE.
    Estrutura de dados:
        Usa uma fila circular (queue_t) para gerenciar a BFS.
        Os componentes são identificados por varredura sequencial do campo.

📌 Melhorias Futuras

    Trocar void* elem por size_t idx em queue_append()
    Modularizar código (separar I/O, lógica de componentes, estruturas)
    Suportar entradas com múltiplos tempos (usar nt de forma dinâmica)
