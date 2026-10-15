#include "ui_model.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace orion_teste;
int main() {
 ViewModel m;
 assert(m.visibleCount()==3);
 assert(m.visibleGameIndex(0)==0 && m.visibleGameIndex(2)==2);
 assert(m.visibleGameIndex(-1)==-1 && m.visibleGameIndex(3)==-1);
 assert(!m.canDownloadSelected());
 m.accept(); assert(m.cardsFocused && m.selectedCard==0);
 m.move(1,0);assert(m.selectedCard==1);
 m.move(1,0);assert(m.selectedCard==2);
 m.move(1,0);assert(m.selectedCard==2); // Clamp.
 m.accept();assert(m.screen==DETAILS);
 m.move(0,1);assert(m.selectedVariant==1);
 m.accept();assert(!m.canDownloadSelected()); // Disabled means no installation.
 m.nextSection(1);assert(m.screen==DETAILS); // Cannot switch sections inside details.
 m.back();assert(m.screen==NAVIGATION);
 m.back();assert(!m.cardsFocused);
 m.nextSection(1);m.nextSection(1);assert(m.section==PLATFORM);
 assert(m.visibleCount()==1 && m.visibleGameIndex(0)==0);
 m.accept();m.accept();assert(m.selectedCard==0 && m.screen==DETAILS);
 m.back();m.back();
 m.nextSection(1); assert(m.section==ARCADE && m.visibleCount()==1 && m.visibleGameIndex(0)==1);
 m.nextSection(1); assert(m.section==PUZZLE && m.visibleGameIndex(0)==2);
 m.nextSection(1);assert(m.section==DOWNLOADS && m.visibleCount()==0);
 m.accept();assert(m.screen==NAVIGATION && !m.cardsFocused);
 m.move(1,0);assert(!m.cardsFocused);
 assert(std::strcmp(languageLabel(NO_VOICES),"SEM VOZES")==0);
 assert(std::strcmp(languageLabel(NO_DIALOGUE),"SEM DIALOGOS")==0);
 assert(std::strcmp(languageLabel(UNKNOWN),"NAO CONFIRMADO")==0);
 for(int i=0;i<3;i++)for(int j=0;j<2;j++)assert(!games[i].variants[j].hasAuthorizedPackage);
 std::cout << "Orion TESTE: modelo nativo de navegacao validado.\n";
}
