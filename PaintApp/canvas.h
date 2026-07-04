#ifndef CANVAS_H
#define CANVAS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "classes.h"

class Canvas {
private:
    sf::FloatRect bounds;
    sf::Color backgroundColor;
    
    // Polymorphic storage of all finalized shapes on canvas
    std::vector<std::unique_ptr<DrawnShape>> shapes;
    
    // Redo history buffer for undo/redo actions
    std::vector<std::unique_ptr<DrawnShape>> redoStack;

public:
    explicit Canvas(sf::FloatRect rect, sf::Color bgCol = sf::Color::White)
        : bounds(rect), backgroundColor(bgCol) {}

    // Renders the background, canvas grid outline, and all drawing shapes
    void draw(sf::RenderTarget& target) const;

    // Adds a shape to canvas, clearing the redo history since a new action occurred
    void addShape(std::unique_ptr<DrawnShape> shape);

    // Moves the last drawn shape from active list to redo stack
    void undo();

    // Restores the last undone shape from redo stack to active list
    void redo();

    // Wipes all shapes and clears history buffers
    void clear();

    // Checks if a given screen point lies within the canvas bounds (preventing drawing on sidebar UI)
    bool containsPoint(sf::Vector2f point) const {
        return bounds.contains(point);
    }

    // Save and load project shapes to/from a custom plain text format
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    // Exports the canvas drawing as a standalone image file (e.g. PNG, JPG)
    bool exportToImage(const std::string& filename) const;

    // Getters for bounds and background color
    sf::FloatRect getBounds() const { return bounds; }
    sf::Color getBackgroundColor() const { return backgroundColor; }
    void setBackgroundColor(sf::Color col) { backgroundColor = col; }

    // Read access to shape vector for inspections
    const std::vector<std::unique_ptr<DrawnShape>>& getShapes() const { return shapes; }
};

#endif // CANVAS_H
