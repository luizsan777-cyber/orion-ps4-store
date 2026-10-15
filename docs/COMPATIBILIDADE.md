# Compatibilidade de firmware: metas e limites

**Não existe garantia de compatibilidade universal.** Um PKG de homebrew necessita de um ambiente que permita executá-lo (por exemplo, GoldHEN ou outro habilitador compatível). Apenas o firmware mais novo ou um PS4 normal não garantem execução de homebrews.

## Alvos de validação

| Firmware | Ambiente alvo | Estado da ORION Store |
|---|---|---|
| 5.05 | GoldHEN compatível | **Não testado** |
| 6.71/6.72 | GoldHEN compatível | **Não testado** |
| 9.00 | GoldHEN compatível | **Não testado** |
| 9.60 | GoldHEN compatível | **Não testado** |
| 10.00/10.01 | GoldHEN compatível | **Não testado** |
| 10.50/10.70/10.71 | GoldHEN compatível | **Não testado** |
| 11.00 | GoldHEN compatível | **Não testado** |
| Acima de 11.00 | Depende de ferramenta de homebrew suportada na versão | **Não garantido** |

**Fonte:** lista de firmware suportado nas notas do GoldHEN: https://github.com/GoldHEN/GoldHEN/releases . Confirme sempre a release concreta antes de ampliar a lista.

## Estratégia

- Renderização SDL2 sem APIs exclusivas de novos firmwares, quando possível.
- Testar a mesma base em cada ambiente homebrew, com detecção de capacidades da API de background/instalação.
- Se um firmware não permitir consulta segura de jogo em primeiro plano, usar confirmação manual para instalar; nunca instalar por suposição.
- Conferir espaço em disco, integridade e interrupções antes de habilitar auto-instalação.
- Medir efetivamente download em segundo plano quando um jogo é aberto e suspenso, quando a console fica na tela inicial e no modo de repouso.

## Casos de testes antes da venda

1. Instalacao limpa do PKG do aplicativo e primeiro uso.
2. Menu totalmente navegável por controle, suporte a idiomas e fontes.
3. Catálogo offline, HTTPS inválido, servidor fora do ar, mudança de redirecionamento, falta de espaço.
4. Dois downloads simultâneos e interrupção Wi-Fi.
5. Retomada depois de reinício, cancelamento seguro, instalação duplicada.
6. Download finaliza durante jogo: notifica mas não inicia instalação intrusiva.
7. Download finaliza na tela inicial: instalação somente após verificar arquivo e confirmar estado do console.
8. Reinício, suspensão e despertar da aplicação.
