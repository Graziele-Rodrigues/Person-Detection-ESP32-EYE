# ESP32 Person Detection - Batch Inference Test

Este projeto é uma modificação do exemplo clássico de detecção de pessoas (**Person Detection**) utilizando o framework **TensorFlow Lite Micro** integrado ao **ESP-IDF**.

O objetivo principal desta versão é rodar uma bateria de testes automatizados com um banco de **51 imagens pré-carregadas** (via CLI em modo estático), coletando métricas cruciais de desempenho, consumo de memória e acurácia do modelo diretamente na saída serial.

## 🚀 Funcionalidades Especiais Desta Versão

* **Modo CLI Only (`CLI_ONLY_INFERENCE = 1`):** Desativa a dependência de câmera física e redireciona o fluxo para ler imagens binárias brutas embutidas diretamente na memória Flash do ESP32.
* **Pipeline de Validação em Lote:** Executa sequencialmente 51 testes (`image0` até `image50`).
* **Saída Estruturada em CSV:** O console serial cospe os dados organizados por vírgula para fácil exportação e análise em ferramentas como Python (Pandas), MATLAB ou Excel.
* **Gerenciamento de Watchdog Otimizado:** Ajustado com yields dinâmicos para prevenir travamentos (`Task Watchdog Trigger`) causados pela alta carga de processamento do modelo e renderizações gráficas.

---

## 📊 Estrutura dos Dados Exportados (Saída CSV)

Durante a execução da função `run_all_images()`, a saída serial imprimirá o cabeçalho e os resultados neste formato:

```text
latency_us,person_score,no_person_score,free_heap_before,free_heap_after,min_heap,predicted,ground_truth,correct

```

### Significado dos Campos:

* `latency_us`: Tempo total que o modelo levou para rodar a inferência (em microssegundos).
* `person_score`: Confiança obtida para a presença de uma pessoa.
* `no_person_score`: Confiança obtida para a ausência de uma pessoa.
* `free_heap_after`: Memória heap interna livre após a execução.
* `min_heap`: O menor nível de heap livre alcançado pelo sistema desde o boot (Watermark).
* `predicted`: Resultado inferido pelo modelo ($1 = \text{Pessoa}$, $0 = \text{Não Pessoa}$).
* `ground_truth`: Gabarito real configurado no array estático.
* `correct`: Status de acerto do modelo ($1 = \text{Acertou}$, $0 = \text{Errou}$).

---

## 🛠️ Especificações Técnicas do Dataset

As imagens embutidas na pasta `static_images/sample_images` seguem rigidamente as especificações do modelo MobileNetV1 0.25 quantizado:

* **Resolução:** $96 \times 96$ pixels.
* **Formato:** Escala de cinza (Grayscale 8-bit).
* **Tipo de Arquivo:** Binário puro (`.raw` / sem cabeçalhos) convertido para o formato *signed int8_t* interno via manipulação de bits (`^ 0x80`).

---

## 📦 Como Compilar e Rodar

### Pré-requisitos

* **ESP-IDF v5.4.2** (ou compatível com a sua branch).
* Placa alvo configurada (ex: **ESP32-S3**).

### Passos para execução:

1. Certifique-se de que os seus 51 arquivos de imagem estejam nomeados de `image0` até `image50` dentro do diretório configurado pelo CMake.
2. Abra o terminal do ESP-IDF e limpe o cache antigo para garantir que o CMake processe todas as novas imagens:
```bash
idf.py fullclean

```

3. Compile o firmware:
```bash
idf.py build

```


4. Grave o código no chip e abra o monitor serial para coletar os resultados:
```bash
idf.py -p COMX flash monitor

```


---

## 🗂️ Organização dos Arquivos Principais

* `main/main.cc`: Inicializa a Task principal do FreeRTOS (`tf_main`) alocando $6\text{ KB}$ de stack.
* `main/main_functions.cc`: Contém a lógica de alocação da `tensor_arena` na memória externa PSRAM (`MALLOC_CAP_SPIRAM`), além da função principal de execução `run_inference()`.
* `main/esp_cli.c`: Gerencia a base de dados estática das imagens, as definições de `ground_truth` e o laço automático de testes.
* `main/esp_main.h`: Arquivo de cabeçalho com as flags de controle (`CLI_ONLY_INFERENCE`).