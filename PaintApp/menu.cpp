#include "menu.h"
#include <iostream>
#include <sstream>

// ==========================================
// Button Implementation
// ==========================================

Button::Button(std::string btnId, sf::Vector2f position, sf::Vector2f size, std::string btnText, const sf::Font& font,
               sf::Color base, sf::Color hover, sf::Color active)
    : id(btnId), baseColor(base), hoverColor(hover), activeColor(active), isSelected(false) {
    
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(baseColor);
    shape.setOutlineColor(sf::Color(30, 30, 40));
    shape.setOutlineThickness(1.f);

    text.setFont(font);
    text.setString(btnText);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    
    // Center text vertically and horizontally in the button bounds
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    text.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f);
}

void Button::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    target.draw(text);
}

bool Button::isHovered(sf::Vector2f mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::update(sf::Vector2f mousePos) {
    if (isSelected) {
        shape.setFillColor(activeColor);
        shape.setOutlineColor(sf::Color(80, 160, 240));
        shape.setOutlineThickness(2.f);
    } else if (isHovered(mousePos)) {
        shape.setFillColor(hoverColor);
        shape.setOutlineColor(sf::Color(200, 200, 200));
        shape.setOutlineThickness(1.f);
    } else {
        shape.setFillColor(baseColor);
        shape.setOutlineColor(sf::Color(45, 45, 55));
        shape.setOutlineThickness(1.f);
    }
}

void Button::setTextString(const std::string& str, const sf::Font& appFont) {
    text.setFont(appFont);
    text.setString(str);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    text.setPosition(shape.getPosition().x + shape.getSize().x / 2.f, shape.getPosition().y + shape.getSize().y / 2.f);
}

// ==========================================
// ColorButton Implementation
// ==========================================

ColorButton::ColorButton(sf::Vector2f position, sf::Vector2f size, sf::Color col)
    : color(col), isSelected(false) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color(45, 45, 55));
    shape.setOutlineThickness(1.f);
}

void ColorButton::draw(sf::RenderTarget& target) const {
    target.draw(shape);
}

bool ColorButton::isHovered(sf::Vector2f mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void ColorButton::update(sf::Vector2f mousePos) {
    if (isSelected) {
        shape.setOutlineColor(sf::Color(255, 255, 255));
        shape.setOutlineThickness(2.5f);
    } else if (isHovered(mousePos)) {
        shape.setOutlineColor(sf::Color(200, 200, 200));
        shape.setOutlineThickness(1.5f);
    } else {
        shape.setOutlineColor(sf::Color(45, 45, 55));
        shape.setOutlineThickness(1.f);
    }
}

// ==========================================
// Menu Implementation
// ==========================================

void Menu::initLabel(sf::Text& label, const std::string& str, float yPos) {
    label.setFont(font);
    label.setString(str);
    label.setCharacterSize(12);
    label.setStyle(sf::Text::Bold);
    label.setFillColor(sf::Color(140, 140, 160));
    label.setPosition(15.f, yPos);
}

Menu::Menu(float width, float height, const sf::Font& appFont)
    : font(appFont), activeToolId("PENCIL"), activeColor(sf::Color::Black), brushThickness(5.f), shapesFilled(false),
      sizeMinusBtn("SIZE_MINUS", {120.f, 280.f}, {35.f, 35.f}, "-", appFont),
      sizePlusBtn("SIZE_PLUS", {165.f, 280.f}, {35.f, 35.f}, "+", appFont),
      fillToggleBtn("FILL_TOGGLE", {15.f, 360.f}, {190.f, 35.f}, "Hollow Shapes", appFont) {
    
    // 1. Configure sidebar structural panel
    sidebarBackground.setSize(sf::Vector2f(width, height));
    sidebarBackground.setPosition(0.f, 0.f);
    sidebarBackground.setFillColor(sf::Color(30, 30, 38)); // Sleek premium dark mode slate
    sidebarBackground.setOutlineColor(sf::Color(45, 45, 55));
    sidebarBackground.setOutlineThickness(1.f);

    // 2. Setup title header
    titleText.setFont(font);
    titleText.setString("MY PAINT");
    titleText.setCharacterSize(22);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color(100, 160, 255)); // Harmonious blue accent
    titleText.setPosition(15.f, 15.f);

    // 3. Initialize section headings
    initLabel(toolsLabel, "DRAWING TOOLS", 65.f);
    initLabel(sizeLabel, "BRUSH WEIGHT", 255.f);
    initLabel(fillLabel, "FILL MODE", 335.f);
    initLabel(colorsLabel, "PALETTE", 410.f);
    initLabel(actionsLabel, "ACTIONS", 510.f);

    // 4. Populate tool grid layout
    toolButtons.push_back(Button("PENCIL", {15.f, 85.f}, {90.f, 35.f}, "Pencil", font));
    toolButtons.push_back(Button("LINE", {115.f, 85.f}, {90.f, 35.f}, "Line", font));
    toolButtons.push_back(Button("RECT", {15.f, 130.f}, {90.f, 35.f}, "Rectangle", font));
    toolButtons.push_back(Button("CIRCLE", {115.f, 130.f}, {90.f, 35.f}, "Circle", font));
    toolButtons.push_back(Button("ERASER", {15.f, 175.f}, {90.f, 35.f}, "Eraser", font));
    toolButtons.push_back(Button("TEXT", {115.f, 175.f}, {90.f, 35.f}, "Text", font));

    // Make pencil default active tool
    toolButtons[0].setSelected(true);

    // 5. Populate brush size layout
    currentSizeText.setFont(font);
    currentSizeText.setString("Size: 5 px");
    currentSizeText.setCharacterSize(16);
    currentSizeText.setFillColor(sf::Color::White);
    currentSizeText.setPosition(15.f, 287.f);

    // 6. Populate color grid layout (6 cols x 2 rows)
    std::vector<sf::Color> colors = {
        sf::Color::Black,
        sf::Color(120, 120, 130),
        sf::Color::White,
        sf::Color(235, 75, 75),     // Smooth Red
        sf::Color(75, 190, 75),     // Clean Green
        sf::Color(65, 130, 240),    // Vibrant Blue
        sf::Color(250, 210, 45),    // Radiant Yellow
        sf::Color(45, 215, 230),    // Indigo Cyan
        sf::Color(235, 65, 195),    // Bright Magenta
        sf::Color(245, 140, 35),    // Soft Orange
        sf::Color(150, 65, 215),    // Elegant Purple
        sf::Color(140, 85, 45)      // Terracotta Brown
    };

    for (size_t i = 0; i < colors.size(); ++i) {
        float x = 15.f + (i % 6) * 32.f;
        float y = 430.f + (i / 6) * 35.f;
        colorButtons.push_back(ColorButton({x, y}, {28.f, 28.f}, colors[i]));
    }

    // Set first color (Black) active
    colorButtons[0].setSelected(true);

    // 7. Populate document actions (undo, clear, save, load, export)
    actionButtons.push_back(Button("UNDO", {15.f, 530.f}, {90.f, 35.f}, "Undo", font));
    actionButtons.push_back(Button("REDO", {115.f, 530.f}, {90.f, 35.f}, "Redo", font));
    actionButtons.push_back(Button("CLEAR", {15.f, 575.f}, {90.f, 35.f}, "Clear", font));
    actionButtons.push_back(Button("SAVE", {115.f, 575.f}, {90.f, 35.f}, "Save", font));
    actionButtons.push_back(Button("LOAD", {15.f, 620.f}, {90.f, 35.f}, "Load", font));
    actionButtons.push_back(Button("EXPORT", {115.f, 620.f}, {90.f, 35.f}, "Export", font));
}

void Menu::draw(sf::RenderTarget& target) const {
    // 1. Draw structural panel background and text headings
    target.draw(sidebarBackground);
    target.draw(titleText);
    target.draw(toolsLabel);
    target.draw(sizeLabel);
    target.draw(fillLabel);
    target.draw(colorsLabel);
    target.draw(actionsLabel);

    // 2. Draw active tool selection controls
    for (const auto& btn : toolButtons) {
        btn.draw(target);
    }

    // 3. Draw brush thickness & fill controls
    target.draw(currentSizeText);
    sizeMinusBtn.draw(target);
    sizePlusBtn.draw(target);
    fillToggleBtn.draw(target);

    // 4. Draw color palette
    for (const auto& btn : colorButtons) {
        btn.draw(target);
    }

    // 5. Draw document utility action buttons
    for (const auto& btn : actionButtons) {
        btn.draw(target);
    }
}

void Menu::update(sf::Vector2f mousePos) {
    // Check and update state highlights for all active buttons
    for (auto& btn : toolButtons) {
        btn.update(mousePos);
    }
    
    sizeMinusBtn.update(mousePos);
    sizePlusBtn.update(mousePos);
    fillToggleBtn.update(mousePos);
    
    for (auto& btn : colorButtons) {
        btn.update(mousePos);
    }
    
    for (auto& btn : actionButtons) {
        btn.update(mousePos);
    }
}

std::string Menu::handleMouseClick(sf::Vector2f mousePos) {
    // 1. Handle tool clicking selection
    for (auto& btn : toolButtons) {
        if (btn.isHovered(mousePos)) {
            // Deselect all tools first
            for (auto& other : toolButtons) {
                other.setSelected(false);
            }
            btn.setSelected(true);
            activeToolId = btn.getId();
            return activeToolId;
        }
    }

    // 2. Handle brush sizing increments
    if (sizeMinusBtn.isHovered(mousePos)) {
        setBrushThickness(brushThickness - 1.f);
        return "SIZE_MINUS";
    }
    if (sizePlusBtn.isHovered(mousePos)) {
        setBrushThickness(brushThickness + 1.f);
        return "SIZE_PLUS";
    }

    // Handle shape fill toggling
    if (fillToggleBtn.isHovered(mousePos)) {
        shapesFilled = !shapesFilled;
        if (shapesFilled) {
            fillToggleBtn.setTextString("Filled Shapes", font);
            fillToggleBtn.setSelected(true);
        } else {
            fillToggleBtn.setTextString("Hollow Shapes", font);
            fillToggleBtn.setSelected(false);
        }
        return "FILL_TOGGLE";
    }

    // 3. Handle color selection
    for (auto& btn : colorButtons) {
        if (btn.isHovered(mousePos)) {
            for (auto& other : colorButtons) {
                other.setSelected(false);
            }
            btn.setSelected(true);
            activeColor = btn.getColor();
            return "COLOR_CHANGED";
        }
    }

    // 4. Handle document utilities action buttons
    for (const auto& btn : actionButtons) {
        if (btn.isHovered(mousePos)) {
            return btn.getId();
        }
    }

    return "";
}

void Menu::setActiveToolId(const std::string& id) {
    activeToolId = id;
    for (auto& btn : toolButtons) {
        btn.setSelected(btn.getId() == id);
    }
}

void Menu::setActiveColor(sf::Color col) {
    activeColor = col;
    // Highlight matching palette item if found
    for (auto& btn : colorButtons) {
        btn.setSelected(btn.getColor() == col);
    }
}

void Menu::setBrushThickness(float thickness) {
    // Enforce reasonable bounds for brush sizing (1px up to 50px)
    if (thickness < 1.f) thickness = 1.f;
    if (thickness > 50.f) thickness = 50.f;
    
    brushThickness = thickness;
    
    // Update structural text
    std::stringstream ss;
    ss << "Size: " << static_cast<int>(brushThickness) << " px";
    currentSizeText.setString(ss.str());
}

void Menu::setShapesFilled(bool filled) {
    shapesFilled = filled;
    if (shapesFilled) {
        fillToggleBtn.setTextString("Filled Shapes", font);
        fillToggleBtn.setSelected(true);
    } else {
        fillToggleBtn.setTextString("Hollow Shapes", font);
        fillToggleBtn.setSelected(false);
    }
}
