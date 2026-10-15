# Orion Store – acompanhamento de desenvolvimento

## 0.2.0-alpha (15 de outubro de 2026)

**Implementado e testado em ambiente de desenvolvimento:**

- Interface web demonstrativa: catálogo, categorias, busca, detalhes e navegação básica com controle via Gamepad API.
- Simulador da regra de instalação em modo demonstrativo (menu / jogando / desconhecido) com aviso explícito de que não instala nada.
- Preview HTML único totalmente offline, sem anúncios e sem abrir sites externos.
- Servidor Node sem dependências de terceiros, endpoints de catálogo, capacidades e health-check.
- Validação de schema e bloqueio de URLs inseguras no catálogo, além de exigência de SHA-256 para entradas de download.
- Máquina de estados JS para fila, pausa/retomada, verificação, integridade e decisões de instalação.
- **Motor nativo C++11 portátil** com estados, progresso, hash aprovado e política de instalação (testado no Linux, ainda não compilado para PS4).
- Testes automatizados de API, catálogo, interface estática e regras em JS e C++.
- Workflow GitHub Actions para testes e publicação opcional da prévia GitHub Pages.

**Não implementado / não comprovado:**

- Instalar o aplicativo no PS4 (não há PKG válido nesta entrega).
- Registrar tarefas BGFT, baixar PKGs, calcular SHA-256 do PKG em streaming no PS4, instalar via AppInstUtil.
- Detectar em tempo real jogo ativo na versão nativa ou emitir notificações de sistema.
- Consultar o catálogo remoto diretamente no aplicativo nativo; ainda contém dados mockados.
- Assinatura digital de manifesto, pinning/rotation de chave, persistência da fila e telemetria opt-in.
- Matriz de testes real por firmware.

## Critérios para chamar 1.0 comercial

1. Abrir PKG da loja em PS4 GoldHEN com firmware testado.
2. Navegar por DualShock 4 em 720p/1080p.
3. Catálogo autorizado carregado por HTTPS e validado.
4. Ação de baixar iniciar transferência direta no PS4 e persistir quando o app fecha, sem navegador.
5. Validar tamanho, integridade e local de origem antes de instalar.
6. Jogando: notificar e aguardar; menu: solicitar instalação segura.
7. Retomar download após queda de rede, tratar falhas e falta de espaço.
8. Testar múltiplos firmwares físicos, documentar limites e obter parecer de licenças/distribuição.
