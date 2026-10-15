# Executar somente apos instalar Git e GitHub CLI e executar: gh auth login
# Cria um repositorio PRIVADO na conta indicada; nao altera outros repositorios.
$ErrorActionPreference = 'Stop'
Set-Location (Split-Path -Parent $PSScriptRoot)
if (-not (Get-Command gh -ErrorAction SilentlyContinue)) { throw 'Instale GitHub CLI: https://cli.github.com/' }
if (-not (Get-Command git -ErrorAction SilentlyContinue)) { throw 'Instale Git: https://git-scm.com/' }
gh auth status
if ($LASTEXITCODE -ne 0) { throw 'Execute gh auth login antes de publicar.' }
if (-not (Test-Path '.git')) {
  git init -b main
  git add .
  git commit -m 'feat: Orion PS4 Store alpha 0.2 - source and tests'
  if ($LASTEXITCODE -ne 0) { throw 'Configure git user.name e user.email antes de publicar.' }
}
gh repo create luizsan777-cyber/orion-ps4-store --private --source . --remote origin --push
if ($LASTEXITCODE -ne 0) { throw 'Nao foi possivel publicar. Confira o nome, permissoes e se o repo ja existe.' }
Write-Output 'Repositorio publicado: https://github.com/luizsan777-cyber/orion-ps4-store'
