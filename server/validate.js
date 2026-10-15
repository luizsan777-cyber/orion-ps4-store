'use strict';
const { isIP } = require('node:net');
const CATEGORIES = new Set(['Acao','Aventura','Arcade','Corrida','Esportes','Estrategia','Luta','Plataforma','Puzzle','RPG','Simulacao','Utilitarios','Outros']);
const isSha256 = v => typeof v === 'string' && /^[a-fA-F0-9]{64}$/.test(v);
function safeDownloadUrl(value){
  if(typeof value !== 'string' || value.length > 2048) return false;
  try {
    const u = new URL(value);
    if(u.protocol !== 'https:' || u.username || u.password || u.port && u.port !== '443') return false;
    const h = u.hostname.toLowerCase();
    if(h === 'localhost' || h.endsWith('.local') || h.endsWith('.localhost') || h.endsWith('.internal') || isIP(h) || !h.includes('.')) return false;
    return true;
  } catch { return false; }
}
function validateCatalog(catalog){
  const errors=[];
  if(!catalog || typeof catalog !== 'object' || catalog.version !== 1 || !Array.isArray(catalog.items)) return ['Esquema de catalogo invalido'];
  const ids=new Set();
  for(const [i,x] of catalog.items.entries()){
    const loc=`items[${i}]`;
    if(!x || typeof x !== 'object'){errors.push(`${loc}: registro invalido`);continue;}
    if(!/^[a-z0-9][a-z0-9-]{1,63}$/.test(x.id||'')) errors.push(`${loc}: id invalido`);
    if(ids.has(x.id))errors.push(`${loc}: id duplicado`);
    ids.add(x.id);
    if(typeof x.title!=='string'||x.title.length<2||x.title.length>90) errors.push(`${loc}: titulo invalido`);
    if(typeof x.description!=='string'||x.description.length>600) errors.push(`${loc}: descricao invalida`);
    if(typeof x.category!=='string'||!CATEGORIES.has(x.category)) errors.push(`${loc}: categoria desconhecida`);
    if(!Number.isSafeInteger(x.sizeBytes)||x.sizeBytes<0) errors.push(`${loc}: tamanho invalido`);
    if(!Array.isArray(x.colors)||x.colors.length!==2||!x.colors.every(c=>/^#[0-9a-fA-F]{6}$/.test(c))) errors.push(`${loc}: cores invalidas`);
    if(x.downloadUrl!==null){
      if(!safeDownloadUrl(x.downloadUrl)) errors.push(`${loc}: URL deve ser HTTPS publico e direto`);
      if(!isSha256(x.sha256)) errors.push(`${loc}: SHA256 obrigatorio`);
      if(typeof x.contentId!=='string'||!/^[A-Za-z0-9_-]{12,60}$/.test(x.contentId)) errors.push(`${loc}: contentId invalido`);
      if(x.sizeBytes<=0) errors.push(`${loc}: tamanho real obrigatorio`);
    }
  }
  return errors;
}
module.exports = { validateCatalog, safeDownloadUrl, CATEGORIES };
