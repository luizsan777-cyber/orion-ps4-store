'use strict';
const {isIP}=require('node:net');
const rights=new Set(['third-party-restricted','original-owned']);
const languages={audioPtBR:new Set(['yes','no','unknown','no-voices']),subtitlesPtBR:new Set(['yes','no','unknown','not-applicable']),interfacePtBR:new Set(['yes','no','unknown'])};
const statuses=new Set(['restricted','pending','preview','available']);
const kinds=new Set(['pkg','browser-preview']);
function publicHttpsUrl(value){
 if(typeof value!=='string'||value.length>2048)return false;
 try {const u=new URL(value);const h=u.hostname.toLowerCase();return u.protocol==='https:'&&!u.username&&!u.password&&(!u.port||u.port==='443')&&h.includes('.')&&!isIP(h)&&!h.endsWith('.local')&&!h.endsWith('.internal')&&!h.endsWith('.localhost')&&h!=='localhost';}catch{return false}
}
function validateEdition(data){
 const errors=[];
 if(!data||data.edition!=='teste'||!Array.isArray(data.items)||data.items.length!==3)return ['Edicao TESTE deve conter exatamente 3 jogos'];
 const ids=new Set();
 data.items.forEach((item,i)=>{
  const at=`items[${i}]`;
  if(!item||typeof item!=='object'){errors.push(`${at}: registro invalido`);return}
  if(!/^[a-z][a-z0-9-]{1,60}$/.test(item.id||''))errors.push(`${at}: id invalido`);
  if(ids.has(item.id))errors.push(`${at}: id duplicado`); ids.add(item.id);
  if(typeof item.title!=='string'||!item.title.trim()||item.title.length>90)errors.push(`${at}: titulo invalido`);
  if(!rights.has(item.publisherRights))errors.push(`${at}: direitos invalidos`);
  if(!Array.isArray(item.accent)||item.accent.length!==2||!item.accent.every(x=>/^#[0-9a-fA-F]{6}$/.test(x)))errors.push(`${at}: cores invalidas`);
  if(!item.language||!Object.entries(languages).every(([field,valid])=>valid.has(item.language[field])))errors.push(`${at}: metadados de idioma invalidos`);
  if(typeof item.language?.note!=='string'||!item.language.note.trim())errors.push(`${at}: faltam evidencias do idioma`);
  if(!Array.isArray(item.variants)||item.variants.length<2||item.variants.length>4)errors.push(`${at}: variantes invalidas`);
  else {
   const vIds=new Set();
   item.variants.forEach((v,j)=>{
    const loc=`${at}.variants[${j}]`;
    if(!v||!kinds.has(v.kind)||!statuses.has(v.status))return errors.push(`${loc}: tipo/estado invalido`);
    if(vIds.has(v.id))errors.push(`${loc}: id duplicado`);vIds.add(v.id);
    if(!v.label||typeof v.label!=='string')errors.push(`${loc}: rotulo invalido`);
    if(v.status==='available'){
      if(v.kind!=='pkg'||item.publisherRights!=='original-owned')errors.push(`${loc}: pacote sem autorizacao comercial`);
      if(!publicHttpsUrl(v.downloadUrl)||!v.downloadUrl.toLowerCase().includes('.pkg'))errors.push(`${loc}: PKG requer URL HTTPS publica e direta`);
      if(!/^[a-f0-9]{64}$/i.test(v.sha256||''))errors.push(`${loc}: SHA-256 ausente`);
      if(!/^[A-Z0-9-_]{12,60}$/i.test(v.contentId||''))errors.push(`${loc}: contentId ausente`);
      if(!Number.isSafeInteger(v.sizeBytes)||v.sizeBytes<=0)errors.push(`${loc}: tamanho ausente`);
    } else if(v.downloadUrl!==null||v.sha256!==null||v.contentId!==null||v.sizeBytes!==null){errors.push(`${loc}: link ou tamanho ficticio nao permitido`)}
    if(v.status==='preview'&&(v.kind!=='browser-preview'||item.publisherRights!=='original-owned'||!['orbit','maze'].includes(item.gameType)))errors.push(`${loc}: preview nao autorizado`);
   });
  }
 });
 if(!ids.has('rayman-legends'))errors.push('Rayman Legends deve aparecer no teste');
 return errors;
}
module.exports={validateEdition,publicHttpsUrl};
