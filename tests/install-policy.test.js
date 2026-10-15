'use strict';
const test=require('node:test');const assert=require('node:assert/strict');
const {decideNextAction}=require('../server/install-policy');
const check=(consoleState,verified=true,ready=true)=>decideNextAction({consoleState,hashVerified:verified,installerReady:ready});
test('quando esta jogando somente notifica',()=>assert.equal(check('gaming').action,'notify'));
test('tela inicial verificada permite instalar',()=>assert.equal(check('home').action,'install'));
test('estado desconhecido nunca instala automaticamente',()=>assert.equal(check('unknown').action,'prompt'));
test('sem hash verificado nao instala',()=>assert.equal(check('home',false).action,'reject'));
test('sem instalador nao instala',()=>assert.equal(check('home',true,false).action,'wait'));
