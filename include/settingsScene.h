#pragma once
#include "scene.h"

namespace SpaceEngine {
    class SettingsScene : public Scene {
    public:
        SettingsScene(PhysicsManager* pPhyManager);
        ~SettingsScene();
        virtual void OnSwitch() override; 
    private:
        void Init();
        virtual void UpdateScene(float dt) override;
        bool goFullscreen(bool enable); 
        bool IncreaseVolume();
        bool DecreaseVolume();

        void UpdateVolumeBar();
        void UpdateVolumeDot();
        void UpdateFullscreenIcons();

        float m_currentVolume = 1.0f;
        bool m_lastFullscreenState = false;
        //per ridimensionare la barra del volume
        Background* m_pVolumeBg = nullptr; 
        Background* m_pVolumeFill = nullptr; //parte colorata
        Button* m_pVolumeKnob = nullptr; //pallino da spostare
        Button* m_pBtnFullOn = nullptr;  //icona fullscreen
        Button* m_pBtnFullOff = nullptr;//icona in finestra
    };
}