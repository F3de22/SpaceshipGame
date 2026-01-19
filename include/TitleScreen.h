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
        virtual void UpdateScene(float dt) override; // Per controllare la risoluzione ogni frame
        int m_lastWidth = 0;
        int m_lastHeight = 0;
        bool StartNewGame(); 
        bool OpenOptions();
        bool OpenLeaderboard();
        bool ExitGame();
    };
}