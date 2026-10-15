# Publicação no GitHub

## Limitação da integração atual

A conexão GitHub usada pelo assistente permite editar repositórios existentes, mas **não oferece a ação de criar um repositório novo**. Por isso, não houve publicação remota nesta entrega; o projeto está organizado localmente, e nenhum repositório alheio foi alterado.

## Caminho mais simples (sem terminal)

1. No navegador, abrir https://github.com/new .
2. Nome: `orion-ps4-store`. Visibilidade: **Private**, durante o desenvolvimento.
3. Criar **sem README, sem .gitignore e sem LICENSE** (arquivos já existem neste projeto).
4. Enviar para o assistente o link do repositório vazio para usar a integração GitHub já conectada e subir o código.

## Comando no Windows (alternativa)

Instale o [Git](https://git-scm.com/) e o [GitHub CLI](https://cli.github.com/). Após `gh auth login`, dentro da pasta do projeto execute em PowerShell:

```powershell
.\scripts\publicar-github.ps1
```

O script cria o repositório privado `luizsan777-cyber/orion-ps4-store`, inicializa o Git e envia o commit. Se o repo já existir, não execute o script de criação; configure o remote e envie o código.

## GitHub Pages

Ao migrar o projeto para um repositório público ou plano compatível, ative `Settings > Pages > Source: GitHub Actions`. O workflow `.github/workflows/preview.yml` constrói e publica a **prévia web estática** para demonstrar interface; não gera PKG, não baixa conteúdo e não concede recursos de PS4.

## Releases

Apenas publique arquivos `.pkg` depois de compilá-los com toolchain PS4 legítima, validar o conteúdo, testar em console real e auditar licenças das dependências.
