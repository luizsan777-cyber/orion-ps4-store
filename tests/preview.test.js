'use strict';
const test=require('node:test');const assert=require('node:assert/strict');
const fs=require('node:fs');const path=require('node:path');const {spawnSync}=require('node:child_process');
test('demo portatil e build estatico nao dependem do backend',()=>{
 const project=path.resolve(__dirname,'..');
 const proc=spawnSync(process.execPath,[path.join(project,'scripts/build-preview.js')],{cwd:project,encoding:'utf8'});
 assert.equal(proc.status,0,proc.stderr);
 const output=fs.readFileSync(path.join(project,'demo.html'),'utf8');
 assert.match(output,/ORION_INLINE_CATALOG/);
 assert.match(output,/Teste da política de instalação/);
 assert.doesNotMatch(output,/<script src=/);
 assert.match(fs.readFileSync(path.join(project,'dist/index.html'),'utf8'),/style\.css/);
});
