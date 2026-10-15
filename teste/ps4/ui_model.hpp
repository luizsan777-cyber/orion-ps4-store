#ifndef ORION_TESTE_UI_MODEL_HPP
#define ORION_TESTE_UI_MODEL_HPP

#include <cstddef>

namespace orion_teste {

enum Section { HOME = 0, ALL_GAMES = 1, PLATFORM = 2, ARCADE = 3, PUZZLE = 4, DOWNLOADS = 5 };
enum Screen { NAVIGATION = 0, DETAILS = 1 };
enum LanguageStatus { UNKNOWN = 0, YES = 1, NO_VOICES = 2, NO_DIALOGUE = 3 };

struct Variant {
    const char* label;
    const char* explanation;
    bool hasAuthorizedPackage;
};

struct Game {
    const char* id;
    const char* title;
    const char* category;
    const char* description;
    LanguageStatus audioPtBr;
    LanguageStatus subtitlesPtBr;
    LanguageStatus interfacePtBr;
    Variant variants[2];
};

extern const Game games[3];
extern const char* sectionNames[6];

struct ViewModel {
    Section section;
    Screen screen;
    bool cardsFocused;
    int selectedCard; // Game index 0..2, never a filtered position.
    int selectedVariant;
    ViewModel();
    int visibleCount() const;
    int visibleGameIndex(int visiblePosition) const;
    int selectedVisiblePosition() const;
    void nextSection(int delta);
    void move(int dx, int dy);
    void accept();
    void back();
    bool canDownloadSelected() const;
};

const char* languageLabel(LanguageStatus value);

} // namespace orion_teste
#endif
