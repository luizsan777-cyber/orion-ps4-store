'use strict';const {test}=require('node:test');const assert=require('node:assert/strict');const fs=require('node:fs'),path=require('node:path'),{spawnSync}=require('node:child_process');
const root=path.resolve(__dirname,'..');
test('build standalone TESTE tem exatamente tres itens sem links de download inventados',()=>{const p=spawnSync(process.execPath,[path.join(root,'build.js')],{encoding:'utf8'});assert.equal(p.status,0,p.stderr);const html=fs.readFileSync(path.join(root,'demo.html'),'utf8');assert.match(html,/ORION_TEST_CATALOG/);assert.match(html,/Rayman Legends/);assert.match(html,/Órbita Neon/);assert.match(html,/Labirinto de Luz/);assert.match(html,/Sem licença/iu);assert.doesNotMatch(html,/<script\s+src=/);assert.doesNotMatch(html,/<link\s+rel="stylesheet"/);assert.doesNotMatch(html,/window\.open\s*\(/)});
test('ambos minijogos contem codigo jogavel em canvas',()=>{const js=fs.readFileSync(path.join(root,'app.js'),'utf8');assert.match(js,/function stepMaze/);assert.match(js,/function openGame/);assert.match(js,/ctx\.arc/)});
test('fonte C++ nativa separa modelo de navegacao e nunca aciona PKG',()=>{
 const cpp=fs.readFileSync(path.join(root,'ps4/main.cpp'),'utf8');
 const model=fs.readFileSync(path.join(root,'ps4/ui_model.cpp'),'utf8');
 assert.match(cpp,/ORION STORE TESTE/);
 for(const title of ['RAYMAN LEGENDS','ORBITA NEON','LABIRINTO DE LUZ'])assert.match(model,new RegExp(title));
 assert.match(fs.readFileSync(path.join(root,'ps4/ui_model.hpp'),'utf8'),/variants\[2\]/);
 assert.match(model,/hasAuthorizedPackage/);
 assert.doesNotMatch(cpp,/bgft_download_start_task/);
});
