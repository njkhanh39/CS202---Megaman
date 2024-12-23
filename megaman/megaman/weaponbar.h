#pragma once
#include "character.h"

class WeaponBar {
private:
    TextureManager* textureManager;

    std::vector<sf::Sprite> weaponIcons; // List of weapon icons
    sf::RectangleShape highlightFrame;   // Frame to highlight selected weapon
    unsigned int selectedWeaponIndex;    // Currently selected weapon
    sf::Vector2f position;               // Position of the weapon bar

public:
    WeaponBar(TextureManager* textureManager, std::vector<short int> weapons) : position(Vector2f(30.f,5.f)), selectedWeaponIndex(0)
    , textureManager(textureManager){

        highlightFrame.setSize({ 16.f, 16.f }); // Set the frame size (same as icon size)
        highlightFrame.setFillColor(sf::Color::Transparent);
        highlightFrame.setOutlineThickness(1.f);
        highlightFrame.setOutlineColor(sf::Color::Yellow);

        this->Load(weapons);
    }

    void Update(Vector2f viewCenter, Vector2f viewSize, short int weapon_id) {
        this->UpdatePosition(viewCenter, viewSize);
        this->UpdateWeapon(weapon_id);
    }

    void Render(sf::RenderTarget* window) {
        for (const auto& icon : weaponIcons) {
            window->draw(icon);
        }
        if (!weaponIcons.empty()) {
            window->draw(highlightFrame);
        }
    }
private:
    void AddWeaponIcon(const std::string& file) {
        sf::Sprite icon;
        sf::Texture* borrow;
        textureManager->BorrowTexture(file, borrow);

        weaponIcons.push_back(icon);
        weaponIcons.back().setTexture(*borrow);
        weaponIcons.back().setPosition(position.x + weaponIcons.size() * 16.f, position.y); // Spacing between icons
    }

    void SelectWeapon(unsigned int index) {
        if (index < weaponIcons.size()) {
            selectedWeaponIndex = index;
            highlightFrame.setPosition(weaponIcons[index].getPosition());
        }
    }

    void UpdateWeapon(short int weapon_id) {
        if (selectedWeaponIndex != weapon_id) SelectWeapon(weapon_id);
    }

    void UpdatePosition(Vector2f viewCenter, Vector2f viewSize) {
        Vector2f screenPos = viewCenter - (viewSize / 2.f) + Vector2f(30.f,5.f);
        this->SetPosition(screenPos);
    }


    void Load(std::vector<short int>& v) {
        for (auto& x : v) {
            if (x == 1) {//xbuster
                AddWeaponIcon("Animation//X//weapon_bar//xbuster.png");
            }
            if (x == 2) {//electric cannon
                AddWeaponIcon("Animation//X//weapon_bar//electric_cannon.png");
            }
            if (x == 3) {//flame thrower
                AddWeaponIcon("Animation//X//weapon_bar//flame_thrower.png");
            }
            if (x == 4) {//ice shotgun
                AddWeaponIcon("Animation//X//weapon_bar//ice_shotgun.png");
            }
        }
    }

    void SetPosition(Vector2f pos) {
        for (int i = 0; i < (int)weaponIcons.size(); ++i) {
            weaponIcons[i].setPosition(Vector2f(pos.x + i * 16.f, pos.y));
        }
        highlightFrame.setPosition(weaponIcons[selectedWeaponIndex].getPosition());
    }
};