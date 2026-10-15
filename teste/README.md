# ORION STORE TESTE — 0.3.0-test

Edição de avaliação **separada** da loja principal. Contém somente **três cartões**:

1. **Rayman Legends (PS4)** — referência de catálogo. É jogo comercial da Ubisoft; **não há PKG autorizado para distribuir**, logo base e atualização aparecem BLOQUEADOS. Página oficial consultada (região Portugal): https://store.playstation.com/pt-pt/product/EP0001-CUSA00031_00-RAYMANLEGENDS001/ — essa listagem não inclui português brasileiro nas vozes/textos. Para a versão/região específica de PS4 pretendida, a presença de português brasileiro está marcada **não confirmada**, em vez de presumida.
2. **Órbita Neon** — minijogo original e pouco conhecido, criado para o teste. Prévia navegável, sem vozes ou diálogos, interface em PT-BR. PKG pendente.
3. **Labirinto de Luz** — minijogo original e pouco conhecido, criado para o teste. Prévia navegável, sem vozes ou diálogos, interface em PT-BR. PKG pendente.

## Experiência

- Cada jogo tem uma tela com **opções independentes de download/edição**. Opções sem fonte autorizada ficam desabilitadas.
- Verificação de idioma separa: **dublagem**, **legendas** e **idioma dos menus**. Os valores possíveis incluem sim, não, não confirmado, sem vozes e sem diálogos.
- Nenhum anúncio; nenhum site externo ou pop-up é aberto.
- Os dois minijogos têm protótipos jogáveis no navegador para verificar fluxo e navegação; **não são PKGs de PS4**.
- Um PKG só poderá aparecer como disponível após publicação de URL HTTPS pública e direta, tamanho real, content ID e SHA-256 verificado, com direitos de distribuição.

## Testar no computador

Requer Node.js 18+. Na pasta `teste/`:

```
npm test
npm run build
```

Abrir `demo.html` no navegador para prévia offline, ou `index.html` via servidor HTTP local. Use teclado (setas, Enter, Esc; / para busca), ou um controle aceito pelo navegador. Nos minijogos, use setas/WASD.

## Compilação PS4

`ps4/main.cpp` é interface experimental SDL2-PS4. A versão 0.4 adiciona `ps4/Makefile`, ícone original, fonte bitmap e o módulo C++11 `ui_model` testável independentemente do SDL2. Não foi compilado no ambiente desta entrega por falta de OpenOrbis/SDL-PS4 configurados. Ainda precisamos integrar BGFT/AppInstUtil, validação real de download, estado do console e notificações. Nada nesta entrega autoriza distribuição de Rayman ou qualquer PKG comercial de terceiros.

## Próximos passos para um teste instalável

1. Configurar OpenOrbis e SDL-PS4 e compilar a **interface teste** com ID de aplicativo independente.
2. Criar PKG de teste e validar boot com GoldHEN 11.00, depois firmwares adicionais suportados.
3. Criar dois **PKGs reais** dos minijogos originais, com conteúdo mínimo, licença adequada, assinatura e checksum.
4. Hospedar PKGs autorizados com HTTPS direto, hash e tamanho verificáveis.
5. Integrar BGFT, pausa/retomada, notificação de download e a regra: instalar apenas quando não estiver jogando.
6. Revisar licenças para distribuição comercial, privacidade e termos de suporte antes de vender.

## Alpha 0.4 — avanço de compilação

Na pasta `teste/ps4/`, `make check` valida se existe uma instalação completa OpenOrbis/SDL-PS4 configurada em `OO_PS4_TOOLCHAIN`; `make` tenta compilar `main.cpp` + `ui_model.cpp` e empacotar um **PKG de interface** (somente quando as ferramentas necessárias existirem). Este Makefile é experimental e pode exigir ajustes à versão real do SDK. O arquivo `sce_sys/about/right.sprx` **não é fornecido** pelo projeto e deve ser obtido de uma distribuição legítima e compatível da toolchain. Os módulos necessários precisam ser obtidos e tratados conforme as licenças.

Executar no computador Linux com compilador C++: `bash ps4/test_model.sh` e `bash ps4/test_sdl_syntax.sh`. O segundo teste usa um *stub de cabeçalhos SDL2*, útil para detectar erros de sintaxe, mas **não prova compatibilidade com PS4 nem execução**. Os testes completos continuam com `npm test`.

**Nenhum PKG validado em hardware, downloader ou instalador foi gerado nesta etapa.** O ícone e a interface da Orion são próprios; o projeto não incorpora arquivos Sony nem distribui Rayman Legends.
