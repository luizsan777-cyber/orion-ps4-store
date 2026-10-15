# ORION Store — Loja homebrew independente para PS4

**Versão 0.2.0-alpha | 15/10/2026**

Projeto de uma loja nativa para PS4 com organização inspirada em lojas de console: início, catálogo, categorias, busca, detalhes, downloads e biblioteca. Identidade visual independente (sem logotipo, ativos, marcas ou associação à Sony/PSN). O foco comercial é a distribuição de **homebrews e PKGs que possam ser legalmente distribuídos**.

## Entregue

- Interface demonstrativa de loja **sem anúncios**, com suporte básico de navegação Gamepad API no navegador.
- Preview em arquivo único `demo.html` que funciona offline; inclui simulador de política de instalação.
- API Node.js sem pacotes externos: `/api/catalog`, `/api/health`, `/api/capabilities`.
- Catálogo JSON validado (seis cartões fictícios, nenhum download de jogo comercial incluído).
- **Lógica de fila/download e política de instalação**, em JavaScript e em C++11 portátil.
- Bloqueio de instalação enquanto o console está jogando; estado desconhecido exige confirmação.
- Base de interface C++/SDL2 PS4 **experimental e ainda não compilada**.
- Workflow de testes e preview estática GitHub Pages.
- **19 testes automatizados aprovados**, incluindo um teste que compila e executa o núcleo C++ em Linux.

## O que NÃO é esta versão

**Não é um PKG funcional nem uma loja comercial pronta para venda.** Não baixa nem instala arquivos no console. Integrações PS4 BGFT/AppInstUtil, consulta remota no app nativo, notificações, assinaturas de manifesto, testes em firmwares e build com OpenOrbis continuam pendentes. Não existe promessa de “zero erros” em redes e servidores.

## Testar no PC

Requisitos: Node.js 18+, compilador C++ (`g++` ou `clang++`) para o teste nativo.

```sh
npm test
npm start
```

Abrir `http://127.0.0.1:4173`. Ou abrir diretamente `demo.html` para prévia offline. Para gerar uma versão estática independente do backend: `npm run build:preview` (pasta `dist/`).

Teclado na prévia: Tab/Enter, setas, / buscar, Esc voltar. Controle via Gamepad API quando suportado. Simulação de política disponível em **Downloads**.

## Estrutura

- `public/` interface web demonstrativa.
- `server/` API, validador, fila e política; sem acesso às APIs PS4.
- `ps4/src/main.cpp` interface nativa preliminar SDL2.
- `ps4/src/core/` núcleo portátil C++11 de fila e regras; testável localmente.
- `ps4/tests/` testes do núcleo C++.
- `tests/` testes Node.js.
- `scripts/` scripts de preview, teste C++ e publicação GitHub (quando o usuário autenticar).
- `docs/` arquitetura, compatibilidade, comercialização, GitHub e histórico.

## Próximas entregas técnicas

1. Preparar build SDL-PS4/OpenOrbis e gerar PKG real de teste.
2. Conectar catálogo remoto à versão nativa e validar TLS/manifesto.
3. Adaptador BGFT/AppInstUtil, controle de espaço e integridade.
4. Detecção confiável de estado do console: menu = solicitação de instalação; jogo = notificação, sem instalação imediata.
5. Testes em 5.05, 6.72, 9.00, 11.00 e outras versões **somente onde homebrew for suportado**, sem compatibilidade universal presumida.
6. Auditoria de licenças, segurança, qualidade e suporte antes da venda.

## Publicar no GitHub

Instruções em [`docs/GITHUB.md`](docs/GITHUB.md). Nenhum repositório remoto é criado automaticamente pelo código.

## Fontes de pesquisa

- https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain
- https://github.com/GoldHEN/GoldHEN
- https://github.com/LightningMods/PS4-Store (GPLv3; **código não copiado**)
- https://github.com/flatz/ps4_remote_pkg_installer (referência de API; **código não copiado**)

## Licença

MIT para o código original deste projeto (ver `LICENSE`). Todas as licenças das dependências nativas e os direitos do conteúdo devem ser auditados antes de distribuição comercial. A licença MIT aqui não concede direito de baixar ou redistribuir jogos comerciais de terceiros.
