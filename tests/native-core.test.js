'use strict';
const test=require('node:test'); const assert=require('node:assert/strict');
const {spawnSync}=require('node:child_process');
const path=require('node:path');
test('motor nativo C++11 de fila e politica de instalacao',()=>{
 const p=spawnSync('bash',[path.join(__dirname,'../scripts/test-native-core.sh')],{encoding:'utf8'});
 if(p.error && p.error.code==='ENOENT') return; // ambiente sem compilador; CI deve provisionar gcc
 assert.equal(p.status,0,`${p.stdout||''}\n${p.stderr||''}`);
 assert.match(p.stdout,/native queue tests: OK/);
});
