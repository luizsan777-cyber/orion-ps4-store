'use strict';
// Cria uma demonstração ESTÁTICA independente de servidor (GitHub Pages).
const fs=require('node:fs'); const path=require('node:path');
const root=path.resolve(__dirname,'..');const output=path.join(root,'dist');
fs.mkdirSync(output,{recursive:true});
for(const name of ['index.html','style.css','app.js'])fs.copyFileSync(path.join(root,'public',name),path.join(output,name));
fs.copyFileSync(path.join(root,'server/catalog.json'),path.join(output,'catalog.json'));
fs.writeFileSync(path.join(output,'.nojekyll'),'');
const index=fs.readFileSync(path.join(root,'public/index.html'),'utf8');
const css=fs.readFileSync(path.join(root,'public/style.css'),'utf8');
const js=fs.readFileSync(path.join(root,'public/app.js'),'utf8');
const catalog=JSON.parse(fs.readFileSync(path.join(root,'server/catalog.json'),'utf8'));
const safeJson=JSON.stringify(catalog).replace(/</g,'\\u003c');
const standalone=index.replace('<link rel="stylesheet" href="./style.css">','<style>'+css+'</style>')
 .replace('<script src="./app.js" defer></script>','<script>window.ORION_INLINE_CATALOG='+safeJson+';</script>\n<script>'+js+'</script>');
fs.writeFileSync(path.join(root,'demo.html'),standalone);
fs.writeFileSync(path.join(output,'demo.html'),standalone);

const api=path.join(output,'api');fs.mkdirSync(api,{recursive:true});
fs.copyFileSync(path.join(root,'server/catalog.json'),path.join(api,'catalog'));
console.log('Prévia estática pronta em dist/ (sem downloads nativos).');
