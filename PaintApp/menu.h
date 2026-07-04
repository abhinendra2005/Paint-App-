#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// General purpose UI text button
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::string id;
    sf::Color baseColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    bool isSelected;

public:
    Button(std::string btnId, sf::Vector2f position, sf::Vector2f size, std::string btnText, const sf::Font& font,
           sf::Color base = sf::Color(55, 55, 65), sf::Color hover = sf::Color(75, 75, 90), sf::Color active = sf::Color(45, 110, 220));

    void draw(sf::RenderTarget& target) const;
    bool isHovered(sf::Vector2f mousePos) const;
    void update(sf::Vector2f mousePos);

    std::string getId() const { return id; }
    void setSelected(bool select) { isSelected = select; }
    bool getSelected() const { return isSelected; }
    void setTextString(const std::string& str, const sf::Font& font);
};

// Specialized button for color selection in the palette grid
class ColorButton {
private:
    sf::RectangleShape shape;
    sf::Color color;
    bool isSelected;

public:
    ColorButton(sf::Vector2f position, sf::Vector2f size, sf::Color col);

    void draw(sf::RenderTarget& target) const;
    bool isHovered(sf::Vector2f mousePos) const;
    void update(sf::Vector2f mousePos);

    sf::Color getColor() const { return color; }
    void setSelected(bool select) { isSelected = select; }
    bool getSelected() const { return isSelected; }
};

// Main Menu container managing the UI sidebar components
class Menu {
private:
    sf::RectangleShape sidebarBackground;
    sf::Font font;
    
    // UI Label Elements
    sf::Text titleText;
    sf::Text toolsLabel;
    sf::Text sizeLabel;
    sf::Text fillLabel;
    sf::Text colorsLabel;
    sf::Text actionsLabel;
    sf::Text currentSizeText;

    // Interactive Button Vectors
    std::vector<Button> toolButtons;
    std::vector<Button> actionButtons;
    std::vector<ColorButton> colorButtons;

    // Active Selection States
    std::string activeToolId;
    sf::Color activeColor;
    float brushThickness;
    bool shapesFilled;

    // Brush Size & Fill Buttons
    Button sizeMinusBtn;
    Button sizePlusBtn;
    Button fillToggleBtn;

    // Helper to position and format standard section labels
    void initLabel(sf::Text& label, const std::string& str, float yPos);

public:
    Menu(float width, float height, const sf::Font& appFont);

    // Render the sidebar UI and all its controls
    void draw(sf::RenderTarget& target) const;

    // Handle mouse movement to update hover states and cursors
    void update(sf::Vector2f mousePos);

    // Handle mouse click triggers and returns the action identifier if clicked, or empty string
    std::string handleMouseClick(sf::Vector2f mousePos);

    // Setters and Getters for drawing states
    std::string getActiveToolId() const { return activeToolId; }
    void setActiveToolId(const std::string& id);

    sf::Color getActiveColor() const { return activeColor; }
    void setActiveColor(sf::Color col);

    float getBrushThickness() const { return brushThickness; }
    void setBrushThickness(float thickness);

    bool getShapesFilled() const { return shapesFilled; }
    void setShapesFilled(bool filled);
};

#endif // MENU_H
