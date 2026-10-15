'use strict';
const fs=require('node:fs'),path=require('node:path');const base=__dirname;
const catalog=JSON.parse(fs.readFileSync(path.join(base,'catalog.json'),'utf8'));
const {validateEdition}=require('./schema');const errors=validateEdition(catalog);if(errors.length)throw Error(errors.join('; '));
const html=fs.readFileSync(path.join(base,'index.html'),'utf8'),css=fs.readFileSync(path.join(base,'style.css'),'utf8'),js=fs.readFileSync(path.join(base,'app.js'),'utf8');
const inline=html.replace('<link rel="stylesheet" href="./style.css">','<style>'+css+'</style>')
 .replace('<script src="./app.js" defer></script>','<script>window.ORION_TEST_CATALOG='+JSON.stringify(catalog).replace(/</g,'\\u003c')+';</script>\n<script>'+js+'</script>');
fs.writeFileSync(path.join(base,'demo.html'),inline);
const dist=path.join(base,'../dist/teste');fs.mkdirSync(dist,{recursive:true});
for(const x of ['index.html','style.css','app.js','catalog.json'])fs.copyFileSync(path.join(base,x),path.join(dist,x));
fs.writeFileSync(path.join(dist,'demo.html'),inline);console.log('Orion TESTE: prévia offline em teste/demo.html; dist/teste/ para publicação estática.');
