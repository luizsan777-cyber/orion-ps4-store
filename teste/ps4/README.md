# Build experimental PS4 — Orion Store TESTE 0.4

O código nativo agora contém uma máquina de navegação C++11 independente de SDL2, renderização 1280×720 em SDL2, Gamepad, três títulos e opções específicas com estado bloqueado. `sce_sys/icon0.png` é um ícone original Orion.

## Verificações locais (Linux)

```bash
bash test_model.sh
bash test_sdl_syntax.sh
```

Estas verificações **não produzem PKG**: `test_sdl_syntax.sh` compila apenas a sintaxe contra um cabeçalho simulado em `tests/stub`, nunca usado no binário PS4.

## Tentativa de PKG real (OpenOrbis instalado)

1. Instale uma versão completa do OpenOrbis PS4 Toolchain para Linux, incluindo os artefatos do SDL-PS4, `create-fself`, `create-gp4` e `PkgTool.Core`.
2. Aponte `OO_PS4_TOOLCHAIN` para a raiz da instalação.
3. Adicione os componentes de módulo/sistema legalmente obtidos e exigidos pelo SDK, incluindo `sce_sys/about/right.sprx` — **não incluídos aqui**.
4. Execute `make check` e `make` nesta pasta.
5. Se o build produzir `IV0000-ORNT00001_00-ORIONSTORETESTE1.pkg`, **trate-o como experimental** até confirmação de boot no PS4 GoldHEN 11.00.

O Makefile segue o fluxo observado em projetos públicos OpenOrbis/SDL2, mas não foi compilado nesta máquina, que não tem o SDK. Verifique as dependências adicionais da instalação SDL-PS4 em caso de falha de link. Os IDs de app estão reservados apenas localmente para o experimento e não possuem registro oficial. Firmwares suportados dependem do ambiente homebrew; não há suporte universal garantido.

### Política de distribuição

A loja não fornece downloads de jogos comerciais não licenciados. Rayman Legends é apenas referência de catálogo e suas opções permanecem bloqueadas. Áudio, legenda e interface PT-BR de Rayman ficam com status **não confirmado** até verificação da SKU específica. As duas outras entradas são protótipos originais em HTML, ainda não convertidos para PKG.
