#ifndef CLASSES_H
#define CLASSES_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

// Base class representing any shape drawn on the canvas
class DrawnShape {
protected:
    sf::Color color;
    float thickness;
    bool isFilled;

public:
    DrawnShape(sf::Color col, float thick, bool filled = false)
        : color(col), thickness(thick), isFilled(filled) {}
    
    virtual ~DrawnShape() = default;

    // Pure virtual functions that every derived shape must implement
    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
    virtual std::string getType() const = 0;

    // Getters and Setters
    sf::Color getColor() const { return color; }
    void setColor(sf::Color col) { color = col; }
    
    float getThickness() const { return thickness; }
    void setThickness(float thick) { thickness = thick; }
    
    bool getIsFilled() const { return isFilled; }
    void setIsFilled(bool filled) { isFilled = filled; }
};

// Represents a freehand drawing path (brush/pencil tool)
class BrushStroke : public DrawnShape {
private:
    std::vector<sf::Vector2f> points;

public:
    BrushStroke(sf::Color col, float thick) 
        : DrawnShape(col, thick, false) {}

    void addPoint(const sf::Vector2f& point) {
        // Prevent adding duplicate points consecutively to reduce rendering load
        if (points.empty() || points.back() != point) {
            points.push_back(point);
        }
    }

    const std::vector<sf::Vector2f>& getPoints() const { return points; }

    void draw(sf::RenderTarget& target) const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    std::string getType() const override { return "BRUSH"; }
};

// Represents a straight line shape
class LineShape : public DrawnShape {
private:
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;

public:
    LineShape(sf::Color col, float thick, sf::Vector2f start = {0.f, 0.f}, sf::Vector2f end = {0.f, 0.f})
        : DrawnShape(col, thick, false), startPoint(start), endPoint(end) {}

    void setStartPoint(sf::Vector2f p) { startPoint = p; }
    void setEndPoint(sf::Vector2f p) { endPoint = p; }
    sf::Vector2f getStartPoint() const { return startPoint; }
    sf::Vector2f getEndPoint() const { return endPoint; }

    void draw(sf::RenderTarget& target) const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    std::string getType() const override { return "LINE"; }
};

// Represents a rectangle shape (hollow or filled)
class RectShape : public DrawnShape {
private:
    sf::Vector2f point1;
    sf::Vector2f point2;

public:
    RectShape(sf::Color col, float thick, bool filled = false, sf::Vector2f p1 = {0.f, 0.f}, sf::Vector2f p2 = {0.f, 0.f})
        : DrawnShape(col, thick, filled), point1(p1), point2(p2) {}

    void setPoint1(sf::Vector2f p) { point1 = p; }
    void setPoint2(sf::Vector2f p) { point2 = p; }
    sf::Vector2f getPoint1() const { return point1; }
    sf::Vector2f getPoint2() const { return point2; }

    void draw(sf::RenderTarget& target) const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    std::string getType() const override { return "RECT"; }
};

// Represents a circle shape (hollow or filled)
class CircleShape : public DrawnShape {
private:
    sf::Vector2f center;
    float radius;

public:
    CircleShape(sf::Color col, float thick, bool filled = false, sf::Vector2f cen = {0.f, 0.f}, float rad = 0.f)
        : DrawnShape(col, thick, filled), center(cen), radius(rad) {}

    void setCenter(sf::Vector2f cen) { center = cen; }
    void setRadius(float rad) { radius = rad; }
    sf::Vector2f getCenter() const { return center; }
    float getRadius() const { return radius; }

    void draw(sf::RenderTarget& target) const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    std::string getType() const override { return "CIRCLE"; }
};

// Represents a text label placed on the canvas
class TextShape : public DrawnShape {
private:
    sf::Vector2f position;
    std::string textStr;
    unsigned int characterSize;

public:
    // Shared font resource set once by main app to render text
    static sf::Font* globalFont;

    TextShape(sf::Color col, float thick = 0.f, sf::Vector2f pos = {0.f, 0.f}, const std::string& str = "", unsigned int charSize = 24)
        : DrawnShape(col, thick, true), position(pos), textStr(str), characterSize(charSize) {}

    void setPosition(sf::Vector2f pos) { position = pos; }
    void setTextString(const std::string& str) { textStr = str; }
    void setCharacterSize(unsigned int size) { characterSize = size; }
    
    sf::Vector2f getPosition() const { return position; }
    std::string getTextString() const { return textStr; }
    unsigned int getCharacterSize() const { return characterSize; }

    void draw(sf::RenderTarget& target) const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    std::string getType() const override { return "TEXT"; }
};

#endif // CLASSES_H
