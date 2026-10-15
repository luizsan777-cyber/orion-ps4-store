'use strict';
/**
 * Motor de fila determinístico. Não baixa arquivos e não acessa APIs do PS4.
 * Eventos são encaminhados por um adaptador nativo *separado* depois de sua validação.
 * A política nunca inicia instalação se houver jogo aberto ou estado desconhecido.
 */
const { decideNextAction } = require('./install-policy');
const STATES = Object.freeze({
  QUEUED:'queued', DOWNLOADING:'downloading', PAUSED:'paused', VERIFYING:'verifying',
  READY:'ready', INSTALLING:'installing', INSTALLED:'installed', ERROR:'error'
});
const allowed = Object.freeze({
  queued:['downloading','error'], downloading:['paused','verifying','error'],
  paused:['downloading','error'], verifying:['ready','error'],
  ready:['installing','error'], installing:['installed','error'],
  installed:[], error:['queued']
});
class DownloadQueue {
  constructor({onEvent=()=>{},now=()=>Date.now()}={}){
    this.tasks=new Map(); this.onEvent=onEvent; this.now=now;
  }
  emit(type,task,details={}){ this.onEvent({type,id:task.id,state:task.state,...details}); }
  enqueue({id,title,contentId,sizeBytes}){
    if(!/^[a-z0-9][a-z0-9-]{1,63}$/.test(id||''))throw Error('invalid_id');
    if(typeof title!=='string'||!title.trim())throw Error('invalid_title');
    if(!Number.isSafeInteger(sizeBytes)||sizeBytes<1)throw Error('invalid_size');
    if(typeof contentId!=='string'||contentId.length<12)throw Error('invalid_content_id');
    const existing=this.tasks.get(id);
    if(existing&&existing.state!==STATES.ERROR)throw Error('duplicate_task');
    const task={id,title,contentId,sizeBytes,state:STATES.QUEUED,transferredBytes:0,verified:false,createdAt:this.now(),updatedAt:this.now(),error:null};
    this.tasks.set(id,task);this.emit('queued',task);return {...task};
  }
  transition(id,next,details={}){
    const task=this.tasks.get(id);if(!task)throw Error('unknown_task');
    if(!allowed[task.state].includes(next))throw Error(`invalid_transition_${task.state}_${next}`);
    if(next===STATES.READY&&!task.verified)throw Error('integrity_not_verified');
    if(next===STATES.INSTALLING&&!task.verified)throw Error('integrity_not_verified');
    task.state=next;task.updatedAt=this.now();task.error=next===STATES.ERROR?String(details.error||'unknown_error'):null;
    this.emit('state_changed',task,{...details});return {...task};
  }
  progress(id,transferredBytes){
    const t=this.tasks.get(id);if(!t||t.state!==STATES.DOWNLOADING)throw Error('not_downloading');
    if(!Number.isSafeInteger(transferredBytes)||transferredBytes<t.transferredBytes||transferredBytes>t.sizeBytes)throw Error('invalid_progress');
    t.transferredBytes=transferredBytes;t.updatedAt=this.now();this.emit('progress',t,{transferredBytes});return {...t};
  }
  verified(id,correctHash){
    const t=this.tasks.get(id);if(!t||t.state!==STATES.VERIFYING)throw Error('not_verifying');
    if(t.transferredBytes!==t.sizeBytes)throw Error('incomplete_download');
    if(correctHash!==true){this.transition(id,STATES.ERROR,{error:'sha256_mismatch'});return {action:'reject'};}
    t.verified=true;this.transition(id,STATES.READY);return {action:'ready'};
  }
  installationDecision(id,{consoleState,installerReady}){
    const t=this.tasks.get(id);if(!t||t.state!==STATES.READY)throw Error('not_ready');
    const decision=decideNextAction({hashVerified:t.verified,consoleState,installerReady});
    if(decision.action==='notify')this.emit('notify_only',t,{reason:decision.reason});
    else if(decision.action==='install')this.emit('request_install',t,{reason:decision.reason});
    else this.emit('pending_install',t,{reason:decision.reason});
    // A chamada real ao instalador só pode ser efetuada por um adaptador PS4.
    return decision;
  }
  snapshot(){return Array.from(this.tasks.values(),t=>({...t}));}
}
module.exports={DownloadQueue,STATES};
