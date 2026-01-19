#include "settingsScene.h"
#include "sceneManager.h"
#include "ui.h"
#include "windowManager.h"
#include "audioManager.h"

namespace SpaceEngine {

    SettingsScene::SettingsScene(PhysicsManager* pPhyManager) : Scene(pPhyManager) {
        Init();
        SPACE_ENGINE_DEBUG("Settings Screen opening...");
    }

    SettingsScene::~SettingsScene() {
        SPACE_ENGINE_DEBUG("Going from Settings to main menu...");
    }

    void SettingsScene::Init() {
        name = "SettingsScene"; 

        UILayout* pUILayout = new UILayout();
        addSceneComponent(pUILayout);
        pUILayout->addComponent<UINavigator>();

        UIMaterial* pBackgroundMat = MaterialManager::createMaterial<UIMaterial>("BGSettings");
        Texture* pTexBGTitleMenu = TextureManager::load(TEXTURES_PATH"backgrounds/Options.png"); 
        pBackgroundMat->addTexture("ui_tex", pTexBGTitleMenu);
        pUILayout->addUIElement(new Background(pBackgroundMat));

        float centerX = 0.f;
        float volumeY = 0.f;
        float fullscreenY = 120.f;
        float backButtonY = 260.f;

        //pulsante volume
        float barXOffset = 40.f;
        UIMaterial* pVolumeMat = MaterialManager::createMaterial<UIButtonMaterial>("VolumeBar");
        Texture* pTexVol = TextureManager::load(TEXTURES_PATH"buttons/grey_bar.png");
        pVolumeMat->addTexture("ui_tex", pTexVol);

        m_pVolumeBg = new Background(pVolumeMat);
        m_pVolumeBg->pUITransf->setAnchor({0.5f, 0.5f}); 
        m_pVolumeBg->pUITransf->setPos({-140.f, 0.f});
        m_pVolumeBg->pUITransf->setFill(false);
        m_pVolumeBg->pUITransf->setWidth(280.f);
        m_pVolumeBg->pUITransf->setHeight(17.f);
        pUILayout->addUIElement(m_pVolumeBg);

        // Riempimento della barra (parte piena/verde)
        UIMaterial* pBarFillMat = MaterialManager::createMaterial<UIMaterial>("VolBarFill");
        Texture* pTexBarFill = TextureManager::load(TEXTURES_PATH"buttons/white_bar.png");
        pBarFillMat->addTexture("ui_tex", pTexBarFill);
        
        m_pVolumeFill = new Background(pBarFillMat);
        m_pVolumeFill->pUITransf->setAnchor({0.5f, 0.5f});
        m_pVolumeFill->pUITransf->setPos({-140.f, 0.f});
        m_pVolumeFill->pUITransf->setFill(false);
        m_pVolumeFill->pUITransf->setWidth(280.f); 
        m_pVolumeFill->pUITransf->setHeight(17.f);
        pUILayout->addUIElement(m_pVolumeFill);

        UIButtonMaterial* pKnobMat = MaterialManager::createMaterial<UIButtonMaterial>("VolKnob");
        Texture* pTexKnob = TextureManager::load(TEXTURES_PATH"buttons/volume_knob.png"); //cerchio bianco
        pKnobMat->addTexture("ui_tex", pTexKnob);

        Button* pKnobBtn = new Button({0.5f, 0.4945f}, {-140.f, 0.f}, pKnobMat, 
            []() { return true; });

        pKnobBtn->onRight = [this]() { return IncreaseVolume(); };
        pKnobBtn->onLeft = [this]() { return DecreaseVolume(); };
        
        pKnobBtn->pUITransf->setWidth(30.f);
        pKnobBtn->pUITransf->setHeight(30.f);

        m_pVolumeKnob = pKnobBtn;
        pUILayout->addUIElement(pKnobBtn);

        //pulsante Fullscreen
        UIButtonMaterial* pFullOnMat = MaterialManager::createMaterial<UIButtonMaterial>("BtnFullscreen");
        Texture* pTexFull = TextureManager::load(TEXTURES_PATH"buttons/Maximize_Window.png");
        pFullOnMat->addTexture("ui_tex", pTexFull);

        m_pBtnFullOn = new Button({0.5f, 0.5f}, {-160.f, fullscreenY}, pFullOnMat, 
            [this]() { return goFullscreen(true); });
        m_pBtnFullOn->pUITransf->setWidth(80.f);
        m_pBtnFullOn->pUITransf->setHeight(80.f);

        //pulsante in finestra
        UIButtonMaterial* pFullOffMat = MaterialManager::createMaterial<UIButtonMaterial>("BtnFullOff");
        Texture* pTexFullOff = TextureManager::load(TEXTURES_PATH"buttons/Minimize_Window.png"); // Tua icona riduci
        pFullOffMat->addTexture("ui_tex", pTexFullOff);

        m_pBtnFullOff = new Button({0.5f, 0.5f}, 
            {70.f, fullscreenY},
            pFullOffMat, 
            [this]() { return goFullscreen(false); });
        m_pBtnFullOff->pUITransf->setWidth(80.f);
        m_pBtnFullOff->pUITransf->setHeight(80.f);
        pUILayout->addUIElement(m_pBtnFullOn);
        pUILayout->addUIElement(m_pBtnFullOff);
        
        // pulsante back
        UIButtonMaterial* pBackMat = MaterialManager::createMaterial<UIButtonMaterial>("BtnBack");
        Texture* pTexBack = TextureManager::load(TEXTURES_PATH"buttons/Back.png");
        pBackMat->addTexture("ui_tex", pTexBack);

        Button* pBtnBack = new Button({0.5f, 0.5f}, {-160.f, backButtonY}, pBackMat, 
            [this]() { SceneManager::SwitchScene("TitleScreen"); return true; });

        pUILayout->addUIElement(pBtnBack);

        UpdateVolumeBar();
        UpdateFullscreenIcons();
    }
    
    void SettingsScene::UpdateScene(float dt)
    {
        if (Mouse::button(SPACE_ENGINE_MOUSE_BUTTON_LEFT))
        {
            float mx = static_cast<float>(Mouse::getPosX());
            float my = static_cast<float>(Mouse::getPosY());

            if (m_pVolumeBg)
            {
                Rect* rect = m_pVolumeBg->pUITransf->getRect();

                float marginY = 30.0f;

                if(mx >= rect->pos.x && mx <= rect->pos.x + rect->size.x &&
                my >= rect->pos.y - marginY && my <= rect->pos.y + rect->size.y + marginY)
                {
                    float relativeX = mx - rect->pos.x;
                    float newVol = relativeX / rect->size.x;

                    if (newVol < 0.0f) newVol = 0.0f;
                    if (newVol > 1.0f) newVol = 1.0f;

                    if (std::abs(m_currentVolume - newVol) > 0.001f)
                    {
                        m_currentVolume = newVol;
                        UpdateVolumeBar();
                        SPACE_ENGINE_DEBUG("Volume Updated: {}", m_currentVolume);
                    }
                }
            }
        }
    }

    bool SettingsScene::goFullscreen(bool enable) {
        bool currentState = WindowManager::fullScreenState;
        
        if (enable && !currentState) {
            WindowManager::ToggleFullScreen();
        } else if (!enable && currentState) {
            WindowManager::ToggleFullScreen(); 
        }
        UpdateFullscreenIcons();
        return true;
    }

    bool SettingsScene::IncreaseVolume()
    {
        m_currentVolume += 0.1f;
        if(m_currentVolume > 1.0f) m_currentVolume = 1.0f;
        
        UpdateVolumeBar();
        return true; 
    }

    bool SettingsScene::DecreaseVolume()
    {
        SPACE_ENGINE_DEBUG("Decrease Volume Pressed! Old Vol: {}", m_currentVolume);
        m_currentVolume -= 0.1f;
        if(m_currentVolume < 0.0f) m_currentVolume = 0.0f;
        
        UpdateVolumeBar();
        return true;
    }

    void SettingsScene::UpdateVolumeBar()
    {
        if(auto am = getAudioManager()) {
        am->SetVolume(m_currentVolume);
    }  

    float startX = -140.f;      // Dove inizia la barra (Fisso)
    float maxBarWidth = 280.f;  // Larghezza totale
    float currentWidth = maxBarWidth * m_currentVolume;

    if(m_pVolumeFill) {
        m_pVolumeFill->pUITransf->setWidth(currentWidth);
        m_pVolumeFill->pUITransf->setPos({startX, 0.f});
    }

    //aggiorna pallino
    if(m_pVolumeKnob) {            
        float knobWidth = 30.0f;
        float knobX = startX + currentWidth - (knobWidth / 2.0f);

        m_pVolumeKnob->pUITransf->setPos({knobX, 0.f});
    }
    }

    void SettingsScene::UpdateFullscreenIcons() {
        bool isFull = WindowManager::fullScreenState;

        if (m_pBtnFullOn && m_pBtnFullOff) {

            UIMaterial* matOn = (UIMaterial*)m_pBtnFullOn->pUIMeshRend->getMaterial();
            UIMaterial* matOff = (UIMaterial*)m_pBtnFullOff->pUIMeshRend->getMaterial();

            if (isFull) {
                if(matOn) matOn->setColor({1.f, 1.f, 1.f, 1.f});  // Luminoso
                if(matOff) matOff->setColor({0.4f, 0.4f, 0.4f, 1.f}); // Opaco/Grigio
            } else {
                if(matOn) matOn->setColor({0.4f, 0.4f, 0.4f, 1.f}); // Opaco/Grigio
                if(matOff) matOff->setColor({1.f, 1.f, 1.f, 1.f});  // Luminoso
            }
        }
    }
}