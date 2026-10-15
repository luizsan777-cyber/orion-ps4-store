'use strict';
const test=require('node:test'); const assert=require('node:assert/strict');
const {loadCatalog,createServer}=require('../server/index'); const {validateCatalog,safeDownloadUrl}=require('../server/validate');
test('catalogo demonstrativo validado',()=>assert.deepEqual(validateCatalog(loadCatalog()),[]));
test('rejeita URLs locais e nao seguras',()=>{
 for(const url of ['http://example.com/test.pkg','https://localhost/test.pkg','https://127.0.0.1/test.pkg','https://user:pass@example.org/p.pkg','https://site.local/game.pkg'])assert.equal(safeDownloadUrl(url),false);
 assert.equal(safeDownloadUrl('https://cdn.example.org/test.pkg'),true);
});
test('rejeita id duplicado',()=>{const c=loadCatalog();c.items.push({...c.items[0]});assert.ok(validateCatalog(c).some(s=>s.includes('duplicado')))});
test('API expõe capacidades honestas e catalogo',async()=>{
 const srv=createServer();await new Promise(r=>srv.listen(0,'127.0.0.1',r));
 try {const base=`http://127.0.0.1:${srv.address().port}`;
 const v=await (await fetch(base+'/api/capabilities')).json(); assert.equal(v.nativeDownloads,false);assert.equal(v.ads,false);
 const c=await (await fetch(base+'/api/catalog')).json();assert.equal(c.items.length,6);
 const x=await fetch(base+'/../package.json');assert.notEqual(x.status,200);
 }finally{await new Promise(r=>srv.close(r));}
});
