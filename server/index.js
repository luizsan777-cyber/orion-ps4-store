'use strict';
const http = require('node:http');
const fs = require('node:fs');
const path = require('node:path');
const { validateCatalog } = require('./validate');
const PORT = Number(process.env.PORT || '4173');
const ROOT=path.resolve(__dirname,'../public');
const CATALOG=path.join(__dirname,'catalog.json');
const TYPES={'.html':'text/html; charset=utf-8','.css':'text/css; charset=utf-8','.js':'application/javascript; charset=utf-8','.svg':'image/svg+xml','.png':'image/png'};
function loadCatalog(){
  const c=JSON.parse(fs.readFileSync(CATALOG,'utf8')); const errors=validateCatalog(c);
  if(errors.length)throw new Error('Catalogo rejeitado: '+errors.join('; '));
  return c;
}
function respond(res,code,data,type='application/json; charset=utf-8'){
  res.writeHead(code,{'Content-Type':type,'X-Content-Type-Options':'nosniff','Cache-Control':'no-store','Content-Security-Policy':"default-src 'self'; img-src 'self' data:; connect-src 'self'; script-src 'self'; style-src 'self'; object-src 'none'; frame-ancestors 'none'"}); res.end(data);
}
function createServer(){
 return http.createServer((req,res)=>{
  const method=req.method||'GET';
  let url; try{url=new URL(req.url,'http://local.invalid')}catch{return respond(res,400,JSON.stringify({error:'URL invalida'}))}
  if(method!=='GET'&&method!=='HEAD')return respond(res,405,JSON.stringify({error:'Metodo nao permitido'}));
  if(url.pathname==='/api/catalog'){
    try{return respond(res,200,JSON.stringify(loadCatalog()))}catch(e){return respond(res,503,JSON.stringify({error:'Catalogo indisponivel'}))}
  }
  if(url.pathname==='/api/capabilities')return respond(res,200,JSON.stringify({mode:'browser_preview',nativeDownloads:false,bgft:false,automaticInstall:false,ads:false,version:'0.2.0'}));
  if(url.pathname==='/api/health')return respond(res,200,JSON.stringify({ok:true}));
  let decoded;try{decoded=decodeURIComponent(url.pathname==='/'?'/index.html':url.pathname)}catch{return respond(res,400,JSON.stringify({error:'URL invalida'}))}
  const p=path.resolve(ROOT,'.'+decoded);
  if(!(p===ROOT||p.startsWith(ROOT+path.sep)))return respond(res,403,JSON.stringify({error:'Acesso negado'}));
  try{
    if(!fs.statSync(p).isFile())return respond(res,404,'Nao encontrado','text/plain');
    const data=fs.readFileSync(p);return respond(res,200,method==='HEAD'?'':data,TYPES[path.extname(p)]||'application/octet-stream');
  }catch{return respond(res,404,'Nao encontrado','text/plain')}
 });
}
if(require.main===module){const server=createServer();server.listen(PORT,'127.0.0.1',()=>console.log(`PS4 Store Preview: http://127.0.0.1:${PORT}`))}
module.exports={createServer,loadCatalog};
