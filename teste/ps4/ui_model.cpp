#include "ui_model.hpp"

namespace orion_teste {

const Game games[3] = {
  {"rayman-legends", "RAYMAN LEGENDS", "PLATAFORMA", "UBISOFT - DISTRIBUICAO NAO AUTORIZADA",
   UNKNOWN, UNKNOWN, UNKNOWN,
   {{"JOGO BASE - INDISPONIVEL", "FALTA LICENCA E PACOTE VERIFICADO", false},
    {"ATUALIZACAO - INDISPONIVEL", "FALTA LICENCA E PACOTE VERIFICADO", false}}},
  {"orbita-neon", "ORBITA NEON", "ARCADE", "MINIJOGO ORIGINAL - PROTOTIPO DISPONIVEL NO PC",
   NO_VOICES, NO_DIALOGUE, YES,
   {{"PREVIA WEB - SOMENTE PC", "ABRA O DEMO.HTML NO COMPUTADOR", false},
    {"PACOTE PS4 - EM DESENVOLVIMENTO", "PKG AINDA NAO GERADO NEM TESTADO", false}}},
  {"labirinto-de-luz", "LABIRINTO DE LUZ", "PUZZLE", "MINIJOGO ORIGINAL - PROTOTIPO DISPONIVEL NO PC",
   NO_VOICES, NO_DIALOGUE, YES,
   {{"PREVIA WEB - SOMENTE PC", "ABRA O DEMO.HTML NO COMPUTADOR", false},
    {"PACOTE PS4 - EM DESENVOLVIMENTO", "PKG AINDA NAO GERADO NEM TESTADO", false}}}
};

const char* sectionNames[6] = {"INICIO", "3 JOGOS", "PLATAFORMA", "ARCADE", "PUZZLE", "DOWNLOADS"};

const char* languageLabel(LanguageStatus value) {
    switch(value) {
    case YES: return "SIM";
    case NO_VOICES: return "SEM VOZES";
    case NO_DIALOGUE: return "SEM DIALOGOS";
    case UNKNOWN: default: return "NAO CONFIRMADO";
    }
}

static bool shown(Section section, int game) {
    switch (section) {
    case PLATFORM: return game == 0;
    case ARCADE: return game == 1;
    case PUZZLE: return game == 2;
    case DOWNLOADS: return false;
    default: return game >= 0 && game < 3;
    }
}

ViewModel::ViewModel(): section(HOME), screen(NAVIGATION), cardsFocused(false), selectedCard(0), selectedVariant(0) {}

int ViewModel::visibleCount() const {
    int n = 0;
    for (int i=0;i<3;i++) if(shown(section,i)) ++n;
    return n;
}

int ViewModel::visibleGameIndex(int position) const {
    if (position < 0) return -1;
    for (int i=0;i<3;i++) if(shown(section,i)) {
        if(position == 0) return i;
        --position;
    }
    return -1;
}

int ViewModel::selectedVisiblePosition() const {
    for (int i=0;i<visibleCount();i++) if(visibleGameIndex(i)==selectedCard) return i;
    return -1;
}

void ViewModel::nextSection(int delta) {
    if (screen==DETAILS) return;
    int next = static_cast<int>(section) + delta;
    if (next < 0) next = 0;
    if (next > 5) next = 5;
    section = static_cast<Section>(next);
    cardsFocused = false;
    selectedCard = visibleGameIndex(0);
    selectedVariant = 0;
}

void ViewModel::move(int dx, int dy) {
    if (screen == DETAILS) {
        if (dy < 0) selectedVariant = 0;
        if (dy > 0) selectedVariant = 1;
        return;
    }
    if (cardsFocused) {
        if (dy < 0) { cardsFocused = false; return; }
        int index = selectedVisiblePosition();
        if (index < 0) index = 0;
        if (dx < 0 && index > 0) --index;
        if (dx > 0 && index < visibleCount()-1) ++index;
        selectedCard = visibleGameIndex(index);
        return;
    }
    if (dy) nextSection(dy > 0 ? 1 : -1);
    if (dx > 0 && visibleCount()>0) {
        cardsFocused=true;
        selectedCard=visibleGameIndex(0);
    }
}

void ViewModel::accept() {
    if (screen == DETAILS) return; // Pending/blocked variants do not trigger downloads.
    if (visibleCount() == 0) return;
    if (!cardsFocused) { cardsFocused=true; selectedCard=visibleGameIndex(0); return; }
    if (selectedVisiblePosition()>=0) { screen=DETAILS; selectedVariant=0; }
}

void ViewModel::back() {
    if (screen == DETAILS) { screen=NAVIGATION; return; }
    cardsFocused=false;
}

bool ViewModel::canDownloadSelected() const {
    if (screen != DETAILS || selectedCard < 0 || selectedCard >= 3) return false;
    // Guard: actual downloadable variants must be configured and validated by a future signed manifest.
    return games[selectedCard].variants[selectedVariant].hasAuthorizedPackage;
}

} // namespace orion_teste
