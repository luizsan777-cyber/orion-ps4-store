'use strict';
const $=s=>document.querySelector(s);const $$=s=>[...document.querySelectorAll(s)];
const state={section:'inicio',category:'Todos',items:[],selected:null,search:'',searchVisible:false,controllerCooldown:0};
const sectionLabels={inicio:'Início',jogos:'Todos os jogos',novidades:'Novidades',categorias:'Categorias',downloads:'Downloads',biblioteca:'Biblioteca',configuracoes:'Configurações'};
function showToast(message){const el=$('#toast');el.textContent=message;el.classList.remove('hidden');clearTimeout(showToast.timer);showToast.timer=setTimeout(()=>el.classList.add('hidden'),3500)}
function setSection(section){state.section=section;state.category='Todos';state.search='';if($('#searchInput'))$('#searchInput').value='';state.searchVisible=false;render()}
function safeText(x){return String(x??'')}
function render(){
 $$('.nav').forEach(b=>{b.classList.toggle('active',b.dataset.section===state.section);b.setAttribute('aria-current',b.dataset.section===state.section?'page':'false')});
 $('#crumb').textContent=sectionLabels[state.section];
 const isCatalog=['inicio','jogos','novidades','categorias'].includes(state.section);
 $('#homeTop').classList.toggle('hidden',state.section!=='inicio');
 $('.heading').classList.toggle('hidden',!isCatalog);$('#chips').classList.toggle('hidden',!isCatalog);$('#tiles').classList.toggle('hidden',!isCatalog);
 $('#searchBox').classList.toggle('hidden',!isCatalog||!state.searchVisible);
 $('#downloadsPanel').classList.toggle('hidden',state.section!=='downloads');
 $('#libraryPanel').classList.toggle('hidden',state.section!=='biblioteca');
 $('#settingsPanel').classList.toggle('hidden',state.section!=='configuracoes');
 if(!isCatalog)return;
 $('#sectionTitle').textContent=state.section==='novidades'?'Adicionados recentemente':state.section==='categorias'?'Escolha uma categoria':'Explore o catálogo';
 $('#kicker').textContent=state.section==='novidades'?'DESTAQUES':'DESCUBRA';
 const categories=['Todos',...new Set(state.items.map(item=>item.category))];
 const chipWrap=$('#chips');chipWrap.replaceChildren();for(const cat of categories){const b=document.createElement('button');b.className='chip'+(state.category===cat?' active':'');b.textContent=cat;b.addEventListener('click',()=>{state.category=cat;render()});chipWrap.append(b)}
 const visible=state.items.filter(item=>(state.category==='Todos'||state.category===item.category)&&(item.title+' '+item.category).toLocaleLowerCase('pt-BR').includes(state.search.toLocaleLowerCase('pt-BR')));
 $('#resultCount').textContent=visible.length+' itens';$('#empty').classList.toggle('hidden',visible.length!==0);const wrap=$('#tiles');wrap.replaceChildren();
 for(const [i,item] of visible.entries()){
  const tile=document.createElement('button');tile.className='tile';tile.dataset.item=item.id;tile.style.setProperty('--c1',item.colors[0]);tile.style.setProperty('--c2',item.colors[1]);
  const art=document.createElement('div');art.className='art';const big=document.createElement('span');big.className='number';big.textContent=String(i+1).padStart(2,'0');const label=document.createElement('span');label.className='artlabel';label.textContent='ORION • '+item.category.toUpperCase();art.append(big,label);
  const copy=document.createElement('div');copy.className='tilecopy';const b=document.createElement('b');b.textContent=item.title;const small=document.createElement('small');small.textContent=item.label||item.category;copy.append(b,small);tile.append(art,copy);tile.addEventListener('click',()=>openDetail(item));wrap.append(tile);
 }
}
function openDetail(item){state.selected=item;$('#detailName').textContent=safeText(item.title);$('#detailCoverName').textContent=safeText(item.title);$('#detailDescription').textContent=safeText(item.description);$('#detailCategory').textContent=safeText(item.category).toUpperCase();$('#detailVersion').textContent='Versão: '+safeText(item.version);$('#detailSize').textContent=item.sizeBytes>0?(item.sizeBytes/1024/1024/1024).toFixed(2)+' GB':'Tamanho: demo';$('#detailAuthor').textContent=safeText(item.author);$('#detailCover').style.setProperty('--c1',item.colors[0]);$('#detailCover').style.setProperty('--c2',item.colors[1]);
 const valid=typeof item.downloadUrl==='string'&&item.downloadUrl.startsWith('https://')&&!!item.sha256;
 $('#detailDownload').disabled=!valid;$('#detailWarning').textContent=valid?'O catálogo tem um link. A instalação no PS4 exige um aplicativo nativo e não pode ser iniciada nesta prévia do navegador.':'Item demonstrativo: não existe PKG nem endereço de download.';$('#detailOverlay').classList.remove('hidden');$('#closeDetail').focus()}
function closeDetail(){$('#detailOverlay').classList.add('hidden');state.selected=null;$('#searchButton').focus()}
function toggleSearch(){if(!['inicio','jogos','novidades','categorias'].includes(state.section)){setSection('jogos')}state.searchVisible=true;$('#searchBox').classList.remove('hidden');$('#searchInput').focus()}
function initEvents(){ $$('.nav').forEach(b=>b.addEventListener('click',()=>setSection(b.dataset.section)));$('#explore').addEventListener('click',()=>setSection('jogos'));$('#searchButton').addEventListener('click',toggleSearch);$('#searchInput').addEventListener('input',e=>{state.search=e.target.value;render()});$('#clearSearch').addEventListener('click',()=>{state.search='';$('#searchInput').value='';render()});$('#closeDetail').addEventListener('click',closeDetail);$('#detailOverlay').addEventListener('click',e=>{if(e.target.id==='detailOverlay')closeDetail()});
 $('#detailDownload').addEventListener('click',()=>showToast('O modo navegador nao baixa PKGs. Nenhum download foi iniciado.'));
 const simMessages={home:'Menu inicial detectado e integridade aprovada: requisitar instalacao ao adaptador PS4 (simulacao; nada sera instalado).',gaming:'Jogo aberto: enviar notificacao de download concluido e aguardar saida do jogo. Sem instalar durante a partida.',unknown:'Nao foi possivel detectar o estado do console: solicitar confirmacao ao usuario. Nunca instalar automaticamente.'};
 for(const [id,state] of [['simHome','home'],['simGame','gaming'],['simUnknown','unknown']]){
  document.getElementById(id).addEventListener('click',()=>{
    document.getElementById('simResult').textContent=simMessages[state];
    for(const b of document.querySelectorAll('.sim-buttons button')) b.classList.toggle('active',b.id===id);
  });
 }
 window.addEventListener('keydown',e=>{if(e.key==='Escape'||e.key==='Backspace'&&e.target.tagName!=='INPUT'){if(!$('#detailOverlay').classList.contains('hidden'))closeDetail();else if(state.searchVisible){state.search='';state.searchVisible=false;render()}else setSection('inicio');e.preventDefault()}if(e.key==='/'&&e.target.tagName!=='INPUT'){toggleSearch();e.preventDefault()}if(e.key==='ArrowRight'||e.key==='ArrowLeft'||e.key==='ArrowDown'||e.key==='ArrowUp'){if(e.target.tagName==='INPUT')return;const eligible=$$('button:not(:disabled)').filter(b=>b.offsetParent!==null);const pos=eligible.indexOf(document.activeElement);if(pos>=0){const next=(pos+(e.key==='ArrowRight'||e.key==='ArrowDown'?1:-1)+eligible.length)%eligible.length;eligible[next].focus();e.preventDefault()}}});
 // Navegadores com Gamepad API: botao X (0) ativa foco; triangulo (3) abre busca; circulo (1) volta.
 let previous=[];function poll(){const g=navigator.getGamepads?.()[0];if(g){const btn=g.buttons.map(b=>b.pressed);if(btn[0]&&!previous[0]&&document.activeElement instanceof HTMLElement)document.activeElement.click();if(btn[1]&&!previous[1]){if(!$('#detailOverlay').classList.contains('hidden'))closeDetail();else setSection('inicio')}if(btn[3]&&!previous[3])toggleSearch();const now=Date.now();if(now-state.controllerCooldown>180){const v=g.axes[1]||0,h=g.axes[0]||0;let key;if(v>.65)key='ArrowDown';else if(v<-.65)key='ArrowUp';else if(h>.65)key='ArrowRight';else if(h<-.65)key='ArrowLeft';if(key){window.dispatchEvent(new KeyboardEvent('keydown',{key,bubbles:true}));state.controllerCooldown=now}}previous=btn}requestAnimationFrame(poll)}requestAnimationFrame(poll);
}
async function boot(){initEvents();try{const load=async()=>{if(window.ORION_INLINE_CATALOG)return window.ORION_INLINE_CATALOG;const r=await fetch('./api/catalog');if(r.ok)return r.json();const fallback=await fetch('./catalog.json');if(!fallback.ok)throw Error('Catálogo indisponível');return fallback.json()};
 const [catalog,cap]=await Promise.all([load(),fetch('./api/capabilities').then(x=>x.ok?x.json():Promise.reject()).catch(()=>({mode:'static_preview',nativeDownloads:false,automaticInstall:false,ads:false}))]);state.items=catalog.items;$('#capabilities').textContent=`Prévia local • download nativo: ${cap.nativeDownloads?'sim':'não'} • instalação automática: ${cap.automaticInstall?'sim':'não'} • anúncios: ${cap.ads?'sim':'não'}`;render()}catch(e){showToast('Falha ao carregar catálogo: '+e.message);$('#capabilities').textContent='Serviço indisponível';render()}}
boot();
