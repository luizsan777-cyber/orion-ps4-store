'use strict';
// Regra de negocio pura, sem executar qualquer chamada ao PS4.
// 'install' so pode ser interpretado pelo futuro adaptador nativo apos
// verificar estado do console e integridade do pacote.
function decideNextAction({hashVerified,consoleState,installerReady}){
  if(!hashVerified)return {action:'reject',reason:'integrity_not_verified'};
  if(!installerReady)return {action:'wait',reason:'installer_unavailable'};
  if(consoleState==='gaming')return {action:'notify',reason:'game_in_progress'};
  if(consoleState==='home')return {action:'install',reason:'home_screen_verified'};
  return {action:'prompt',reason:'console_state_unknown'};
}
module.exports={decideNextAction};
