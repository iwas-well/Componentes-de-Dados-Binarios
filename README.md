# Máscara Automática de Componentes em Dados Meteorológicos

(TO ADD: Alteração feita para usar como input dados com 12 tempos. saida possui conjuntos de 12 tempos, cada conjunto n com as 12 n-esimas mascaras de cada timestep da entrada)

Este programa em C identifica componentes conexos em campos meteorológicos armazenados em formato binário. Os componentes são definidos como conjuntos de células adjacentes com mesmo sinal (valor positivo ou negativo), formando regiões conectadas.

O dado é tratado como um grafo não direcionado, onde cada célula é um vértice com até 8 vizinhos (adjacência de Moore). O programa gera automaticamente máscaras binárias (em novos arquivos binários) para os componentes com tamanho superior a um limiar mínimo.

# Entrada
- Um arquivo binário de entrada contendo nx * ny * nt floats, representando um campo bidimensional com valores positivos e negativos.

# Saída
- Um arquivo binário com até MAX_OUT_NT (padrão: 30) campos bidimensionais (nx × ny).
    - Cada campo representa uma máscara com valores:
        1.0 para células pertencentes ao componente detectado.
        <undef> para demais posições (valor indefinido passado por parâmetro).
    - Cada máscara representa um componente de tamanho ≥ MIN_COMPONENT_SIZE (padrão: 80).
    - O número de campos escritos é printado na saída padrão ao fim do programa.

# Uso
```
./mascara_componentes <arquivo_entrada> <nx> <ny> <nt> <undef> <arquivo_saida>
```
Exemplo:
```
./mascara_componentes dados/input.bin 58 71 1 777.7 saida.bin
```

# Como Funciona
- Leitura dos dados binários: O programa lê nx * ny valores float de um arquivo.
- Busca por componentes conexos:
    - Para cada célula ainda não visitada:
    - Executa uma BFS para encontrar todas as células conectadas com o mesmo sinal (positivo ou negativo) ignorando células com valor indefinido.
    - Marca todas as células do componente com valor 1.0 na máscara de saída.
    - Salva a máscara se o componente tiver tamanho ≥ MIN_COMPONENT_SIZE.
- Estrutura de dados utilizada:
    Usa uma fila circular (queue_t) para gerenciar a BFS.
    Os componentes são identificados por varredura sequencial do campo.

# Melhorias Futuras
- Trocar void* elem por size_t idx em queue_append()
- Modularizar código (separar I/O, lógica de componentes, estruturas)
- Suportar entradas com múltiplos tempos (usar nt de forma dinâmica)
