#pragma once
#include <vector>
#include "scene.h"

namespace SpaceEngine{

    enum class TitleResult {
        NONE,           // Nessuna azione (l'utente sta solo muovendo il mouse)
        PLAY,           // Ha cliccato "Nuova Partita"
        OPTIONS,        // Ha cliccato "Opzioni"
        LEADERBOARD,    // Ha cliccato "Classifica"
        EXIT            // Ha cliccato "Esci"
    };

    class TitleScreen : public Scene
    {
    public:
        TitleScreen(PhysicsManager* pPhyManager);
        ~TitleScreen();
    private:
        void Init();
        //bool per avere conferma che la funzione sia stata lanciata 
        //correttamente
        bool StartNewGame(); 
        bool OpenOptions();
        bool OpenLeaderboard();
        bool ExitGame();
    };
}