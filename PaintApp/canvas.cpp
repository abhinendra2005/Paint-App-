#include "canvas.h"
#include <fstream>
#include <iostream>

void Canvas::draw(sf::RenderTarget& target) const {
    // 1. Draw Canvas background sheet
    sf::RectangleShape bgRect(sf::Vector2f(bounds.width, bounds.height));
    bgRect.setPosition(bounds.left, bounds.top);
    bgRect.setFillColor(backgroundColor);
    
    // Draw subtle border around canvas boundary
    bgRect.setOutlineColor(sf::Color(200, 200, 200));
    bgRect.setOutlineThickness(2.f);
    target.draw(bgRect);

    // 2. Draw all shapes that exist on canvas
    for (const auto& s : shapes) {
        s->draw(target);
    }
}

void Canvas::addShape(std::unique_ptr<DrawnShape> shape) {
    if (shape) {
        shapes.push_back(std::move(shape));
        redoStack.clear(); // Standard behavior: clear redo stack when a new action is performed
    }
}

void Canvas::undo() {
    if (!shapes.empty()) {
        redoStack.push_back(std::move(shapes.back()));
        shapes.pop_back();
    }
}

void Canvas::redo() {
    if (!redoStack.empty()) {
        shapes.push_back(std::move(redoStack.back()));
        redoStack.pop_back();
    }
}

void Canvas::clear() {
    shapes.clear();
    redoStack.clear();
}

bool Canvas::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return false;
    }

    // Write all shapes in order
    for (const auto& s : shapes) {
        s->serialize(ofs);
    }

    return true;
}

bool Canvas::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return false;
    }

    // Clear active drawing and undo buffers
    shapes.clear();
    redoStack.clear();

    std::string type;
    while (ifs >> type) {
        std::unique_ptr<DrawnShape> shape = nullptr;

        // Instantiate corresponding derived class polymorphically
        if (type == "BRUSH") {
            shape = std::make_unique<BrushStroke>(sf::Color::Black, 1.f);
        } else if (type == "LINE") {
            shape = std::make_unique<LineShape>(sf::Color::Black, 1.f);
        } else if (type == "RECT") {
            shape = std::make_unique<RectShape>(sf::Color::Black, 1.f);
        } else if (type == "CIRCLE") {
            shape = std::make_unique<CircleShape>(sf::Color::Black, 1.f);
        } else if (type == "TEXT") {
            shape = std::make_unique<TextShape>(sf::Color::Black, 1.f);
        }

        if (shape) {
            shape->deserialize(ifs);
            shapes.push_back(std::move(shape));
        }
    }

    return true;
}

bool Canvas::exportToImage(const std::string& filename) const {
    // 1. Create render texture matching canvas dimension
    sf::RenderTexture rt;
    if (!rt.create(static_cast<unsigned int>(bounds.width), static_cast<unsigned int>(bounds.height))) {
        std::cerr << "Failed to create RenderTexture for exporting." << std::endl;
        return false;
    }

    // 2. Clear canvas with background color
    rt.clear(backgroundColor);

    // 3. Setup view translating window coordinates to canvas-relative coordinates
    sf::View view(sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height));
    rt.setView(view);

    // 4. Render all shapes onto the texture
    for (const auto& s : shapes) {
        s->draw(rt);
    }

    // 5. Finalize rendering and capture image
    rt.display();
    sf::Image img = rt.getTexture().copyToImage();

    // 6. Save image to disk
    return img.saveToFile(filename);
}
