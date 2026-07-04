#include "classes.h"
#include <cmath>

// Initialize static member
sf::Font* TextShape::globalFont = nullptr;

// ==========================================
// BrushStroke Implementation
// ==========================================

void BrushStroke::draw(sf::RenderTarget& target) const {
    if (points.empty()) return;

    // Draw circular caps at each node for smooth rendering
    sf::CircleShape cap(thickness / 2.f);
    cap.setFillColor(color);
    cap.setOrigin(thickness / 2.f, thickness / 2.f);

    if (points.size() == 1) {
        cap.setPosition(points[0]);
        target.draw(cap);
        return;
    }

    for (size_t i = 0; i < points.size(); ++i) {
        cap.setPosition(points[i]);
        target.draw(cap);

        // Draw rotated rectangular segments connecting consecutive points
        if (i < points.size() - 1) {
            sf::Vector2f p1 = points[i];
            sf::Vector2f p2 = points[i + 1];
            sf::Vector2f dir = p2 - p1;
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            
            if (length > 0.1f) {
                sf::RectangleShape rect(sf::Vector2f(length, thickness));
                rect.setFillColor(color);
                rect.setOrigin(0.f, thickness / 2.f);
                rect.setPosition(p1);
                
                float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
                rect.setRotation(angle);
                target.draw(rect);
            }
        }
    }
}

void BrushStroke::serialize(std::ostream& os) const {
    os << getType() << " " 
       << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << " "
       << thickness << " " << points.size();
    for (const auto& p : points) {
        os << " " << p.x << " " << p.y;
    }
    os << "\n";
}

void BrushStroke::deserialize(std::istream& is) {
    int r, g, b, a;
    size_t size;
    is >> r >> g >> b >> a >> thickness >> size;
    color = sf::Color(r, g, b, a);
    points.resize(size);
    for (size_t i = 0; i < size; ++i) {
        is >> points[i].x >> points[i].y;
    }
}

// ==========================================
// LineShape Implementation
// ==========================================

void LineShape::draw(sf::RenderTarget& target) const {
    sf::Vector2f dir = endPoint - startPoint;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length < 0.1f) return;

    // Draw connecting rectangle
    sf::RectangleShape rect(sf::Vector2f(length, thickness));
    rect.setFillColor(color);
    rect.setOrigin(0.f, thickness / 2.f);
    rect.setPosition(startPoint);
    
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    rect.setRotation(angle);
    target.draw(rect);

    // Draw circular ends for sleek anti-aliasing caps
    sf::CircleShape cap(thickness / 2.f);
    cap.setFillColor(color);
    cap.setOrigin(thickness / 2.f, thickness / 2.f);
    
    cap.setPosition(startPoint);
    target.draw(cap);
    
    cap.setPosition(endPoint);
    target.draw(cap);
}

void LineShape::serialize(std::ostream& os) const {
    os << getType() << " " 
       << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << " "
       << thickness << " " << startPoint.x << " " << startPoint.y << " " << endPoint.x << " " << endPoint.y << "\n";
}

void LineShape::deserialize(std::istream& is) {
    int r, g, b, a;
    is >> r >> g >> b >> a >> thickness >> startPoint.x >> startPoint.y >> endPoint.x >> endPoint.y;
    color = sf::Color(r, g, b, a);
}

// ==========================================
// RectShape Implementation
// ==========================================

void RectShape::draw(sf::RenderTarget& target) const {
    float x = std::min(point1.x, point2.x);
    float y = std::min(point1.y, point2.y);
    float w = std::abs(point2.x - point1.x);
    float h = std::abs(point2.y - point1.y);
    
    if (w < 1.f && h < 1.f) return;

    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    
    if (isFilled) {
        rect.setFillColor(color);
        rect.setOutlineThickness(0.f);
    } else {
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(color);
        rect.setOutlineThickness(-thickness); // Outlines stay strictly inside bounding box
    }
    target.draw(rect);
}

void RectShape::serialize(std::ostream& os) const {
    os << getType() << " " 
       << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << " "
       << thickness << " " << (isFilled ? 1 : 0) << " "
       << point1.x << " " << point1.y << " " << point2.x << " " << point2.y << "\n";
}

void RectShape::deserialize(std::istream& is) {
    int r, g, b, a, filled;
    is >> r >> g >> b >> a >> thickness >> filled >> point1.x >> point1.y >> point2.x >> point2.y;
    color = sf::Color(r, g, b, a);
    isFilled = (filled != 0);
}

// ==========================================
// CircleShape Implementation
// ==========================================

void CircleShape::draw(sf::RenderTarget& target) const {
    if (radius < 1.f) return;

    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(center);
    
    if (isFilled) {
        circle.setFillColor(color);
        circle.setOutlineThickness(0.f);
    } else {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(-thickness); // Outlines stay strictly inside circle radius
    }
    target.draw(circle);
}

void CircleShape::serialize(std::ostream& os) const {
    os << getType() << " " 
       << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << " "
       << thickness << " " << (isFilled ? 1 : 0) << " "
       << center.x << " " << center.y << " " << radius << "\n";
}

void CircleShape::deserialize(std::istream& is) {
    int r, g, b, a, filled;
    is >> r >> g >> b >> a >> thickness >> filled >> center.x >> center.y >> radius;
    color = sf::Color(r, g, b, a);
    isFilled = (filled != 0);
}

// ==========================================
// TextShape Implementation
// ==========================================

void TextShape::draw(sf::RenderTarget& target) const {
    if (globalFont && !textStr.empty()) {
        sf::Text text;
        text.setFont(*globalFont);
        text.setString(textStr);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);
        text.setPosition(position);
        target.draw(text);
    }
}

void TextShape::serialize(std::ostream& os) const {
    os << getType() << " " 
       << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << " "
       << thickness << " " << position.x << " " << position.y << " " << characterSize << " "
       << textStr.length() << " " << textStr << "\n";
}

void TextShape::deserialize(std::istream& is) {
    int r, g, b, a;
    size_t length;
    is >> r >> g >> b >> a >> thickness >> position.x >> position.y >> characterSize >> length;
    color = sf::Color(r, g, b, a);
    
    char space;
    is.get(space); // Consume the space separator
    
    textStr.resize(length);
    is.read(&textStr[0], length);
}
