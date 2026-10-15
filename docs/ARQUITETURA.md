# Arquitetura de implementação do PS4

```text
PS4 (interface nativa SDL2)
  -> HTTPS API catálogo remoto
    -> itens autorizados: título, categoria, URL PKG HTTPS, bytes, hash SHA-256, content ID
  -> validação do manifesto e do servidor
  -> registrador de tarefa BGFT/AppInstUtil (futuro adaptador nativo)
  -> fila gerenciada pelo sistema PS4 (quando o ambiente permitir)
  -> notificação / solicitação de instalação / progresso
```

A aplicação deve evitar WebView com anúncios. A URL direta é acessada pelo sistema de transferência, não como uma página visitada.

## Estados de tarefa propostos

`aguardando -> validando -> transferindo -> verificando -> pronto_para_instalar -> instalando -> concluido`;

Falhas possíveis: `sem_espaco`, `servidor_inacessivel`, `url_nao_direta`, `hash_incorreto`, `bgft_indisponivel`, `ja_instalado`, `interrompido`. Não mascarar erro BGFT como concluído.

## Política de instalação do usuário

- **PS4 na tela inicial:** após download + verificação, tentar instalação automática SE for possível detectar o estado do console de maneira confiável.
- **PS4 durante um jogo:** registrar somente notificação de que está pronto para instalar; aguardar saída do jogo.
- Sem API confiável para detectar a condição: exibir ação manual de instalar, preferível a alterar o estado de um jogo aberto.

## Segurança e estabilidade

- HTTPS/TLS validado (nunca desabilitar validação de certificado).
- Arquivos grandes: não carregar na memória; registrar tarefa BGFT ou usar stream/resume, quando implementado.
- Sempre comparar bytes recebidos e SHA-256 quando o provedor fornecer checksum confiável.
- Rejeitar links `javascript:`, páginas HTML, URLs locais inesperadas, credenciais em query e redirecionamentos maliciosos.
- Controle de erros, timeout, idempotência, fila persistente, registros de depuração e telemetria opt-in.

## Fontes de referência

- OpenOrbis: https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain
- PS4-Store LightningMods (GPLv3): https://github.com/LightningMods/PS4-Store
- Exemplo BGFT histórico: https://github.com/flatz/ps4_remote_pkg_installer

O exemplo BGFT utiliza interfaces sensíveis à versão do firmware; não foi incorporado até verificar API, licenças e teste real. Não atribuir ao protótipo capacidades ainda não implementadas.
