# Interface nativa experimental do PS4 (SDL2)

Este diretório contém o inicio de uma implementação **nativa** em C++/SDL2, separada da prévia web. Ela já possui menu lateral, grade, categorias, detalhes e eventos para DualShock 4 via SDL_GameController.

**Estado atual:** sem instalação de PKGs, sem downloads nativos, sem consulta de catálogo remoto e sem notificação de sistema. O próprio app informa isso. É um MVP de interface que ainda precisa ser compilado para PS4 com SDK compatível e testado em hardware real.

## Compilação planejada

1. Instalar a toolchain [OpenOrbis PS4](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain) com SDL2-PS4, compilador, linker, create-fself e PkgTool.
2. Usar o sample SDL2 disponibilizado pela toolchain como base de build e empacotamento, copiando `src/main.cpp` para a etapa de compilação e `assets/font.bmp` para `/app0/assets/font.bmp` no pacote.
3. Compilar, empacotar e testar a instalação no PS4 em versões de firmware com homebrew habilitado.
4. Somente depois ligar a implementacao auditada de BGFT/servico de instalacao, fazendo testes de download em segundo plano.

Nenhum PKG foi compilado neste ambiente porque a toolchain não está instalada; gerar um arquivo com extensão `.pkg` sem binário executável válido seria enganoso.

## Comportamento a implementar na integração PS4

- Validar URL HTTPS, tamanho real e hash SHA-256 do arquivo autorizado.
- Criar tarefa BGFT da transferência usando o serviço de download do PS4.
- Exibir status e erros da tarefa, permitir cancelar e retomar conforme suporte do firmware.
- Com o jogo aberto, emitir notificação **quando a API permitir** e não iniciar instalação intrusiva.
- Na tela inicial, solicitar instalação automaticamente **após** verificação de integridade; se detecção confiável não existir, mostrar ação explícita ao usuário.
- Não fazer polling/injeção de processos do jogo nem usar bypass de proteções de download.

## Núcleo C++ testável independentemente

O arquivo `src/core/download_queue.cpp` implementa a lógica da fila e o modelo de decisão `ConsoleState::Home / Gaming / Unknown`. Para compilar e validar esse motor em Linux com C++11:

```sh
../scripts/test-native-core.sh
```

O sucesso dos testes em PC **não implica** sucesso da integração com as APIs do PS4. O adaptador BGFT/AppInstUtil permanece pendente.
