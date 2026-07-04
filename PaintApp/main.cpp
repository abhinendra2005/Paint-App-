#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "classes.h"
#include "canvas.h"
#include "menu.h"

int main() {
    // 1. Create main application window (720p resolution)
    sf::RenderWindow window(sf::VideoMode(1280, 720), "My Paint", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60); // Clean, paced frames at 60 FPS

    // 2. Load Font with automatic robust fallbacks for standard Windows platforms
    sf::Font font;
    bool fontLoaded = false;
    
    if (font.loadFromFile("arial.ttf")) {
        fontLoaded = true;
    } else if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        fontLoaded = true;
    } else if (font.loadFromFile("C:\\Windows\\Fonts\\segoeui.ttf")) {
        fontLoaded = true;
    }

    if (!fontLoaded) {
        std::cerr << "CRITICAL ERROR: No suitable system font (Arial/Segoe UI) could be found on disk.\n"
                  << "Text drawing tools will be disabled.\n";
    } else {
        // Set the global font pointer inside TextShape class to enable rendering
        TextShape::globalFont = &font;
    }

    // 3. Initialize components
    // Sidebar takes x = [0, 220], Canvas takes x = [220, 1280]
    Canvas canvas(sf::FloatRect(220.f, 0.f, 1060.f, 720.f));
    Menu menu(220.f, 720.f, font);

    // 4. State tracking variables
    bool isDrawing = false;
    std::unique_ptr<DrawnShape> currentShape = nullptr;
    
    // Text drawing state variables
    bool isTyping = false;
    sf::Vector2f textPosition(0.f, 0.f);
    std::string activeTextString = "";

    // 5. Main Application Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle close request
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Mouse button pressed events
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    // A. Clicked on Sidebar Menu
                    if (mousePosF.x < 220.f) {
                        std::string action = menu.handleMouseClick(mousePosF);

                        // Trigger corresponding canvas action
                        if (action == "UNDO") {
                            canvas.undo();
                        } else if (action == "REDO") {
                            canvas.redo();
                        } else if (action == "CLEAR") {
                            canvas.clear();
                        } else if (action == "SAVE") {
                            canvas.saveToFile("drawing_project.paint");
                            std::cout << "Project saved successfully to drawing_project.paint\n";
                        } else if (action == "LOAD") {
                            if (canvas.loadFromFile("drawing_project.paint")) {
                                std::cout << "Project loaded successfully from drawing_project.paint\n";
                            }
                        } else if (action == "EXPORT") {
                            if (canvas.exportToImage("drawing.png")) {
                                std::cout << "Canvas exported successfully to drawing.png\n";
                            }
                        }

                        // Close active text writing if user clicks sidebar menu
                        if (isTyping) {
                            if (!activeTextString.empty()) {
                                canvas.addShape(std::make_unique<TextShape>(
                                    menu.getActiveColor(), 0.f, textPosition, activeTextString, 24
                                ));
                            }
                            isTyping = false;
                            activeTextString = "";
                        }
                    }
                    // B. Clicked on Canvas area
                    else if (canvas.containsPoint(mousePosF)) {
                        std::string tool = menu.getActiveToolId();

                        if (tool == "TEXT") {
                            // If already typing, commit the current text first before shifting text positions
                            if (isTyping && !activeTextString.empty()) {
                                canvas.addShape(std::make_unique<TextShape>(
                                    menu.getActiveColor(), 0.f, textPosition, activeTextString, 24
                                ));
                            }
                            isTyping = true;
                            textPosition = mousePosF;
                            activeTextString = "";
                        } else {
                            // Commit any text if transitioning from typing to shape drawing
                            if (isTyping) {
                                if (!activeTextString.empty()) {
                                    canvas.addShape(std::make_unique<TextShape>(
                                        menu.getActiveColor(), 0.f, textPosition, activeTextString, 24
                                    ));
                                }
                                isTyping = false;
                                activeTextString = "";
                            }

                            // Start shape dragging
                            isDrawing = true;
                            sf::Color color = menu.getActiveColor();
                            float thick = menu.getBrushThickness();

                            if (tool == "PENCIL") {
                                auto brush = std::make_unique<BrushStroke>(color, thick);
                                brush->addPoint(mousePosF);
                                currentShape = std::move(brush);
                            } else if (tool == "ERASER") {
                                // Eraser acts as a white brush stroke matching the canvas background color
                                auto eraser = std::make_unique<BrushStroke>(canvas.getBackgroundColor(), thick);
                                eraser->addPoint(mousePosF);
                                currentShape = std::move(eraser);
                            } else if (tool == "LINE") {
                                currentShape = std::make_unique<LineShape>(color, thick, mousePosF, mousePosF);
                            } else if (tool == "RECT") {
                                currentShape = std::make_unique<RectShape>(color, thick, menu.getShapesFilled(), mousePosF, mousePosF);
                            } else if (tool == "CIRCLE") {
                                currentShape = std::make_unique<CircleShape>(color, thick, menu.getShapesFilled(), mousePosF, 0.f);
                            }
                        }
                    }
                }
            }

            // Mouse movement events
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosF(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));

                if (isDrawing && currentShape) {
                    std::string tool = menu.getActiveToolId();

                    if (tool == "PENCIL" || tool == "ERASER") {
                        auto* brush = dynamic_cast<BrushStroke*>(currentShape.get());
                        if (brush && canvas.containsPoint(mousePosF)) {
                            brush->addPoint(mousePosF);
                        }
                    } else if (tool == "LINE") {
                        auto* line = dynamic_cast<LineShape*>(currentShape.get());
                        if (line) {
                            line->setEndPoint(mousePosF);
                        }
                    } else if (tool == "RECT") {
                        auto* rect = dynamic_cast<RectShape*>(currentShape.get());
                        if (rect) {
                            rect->setPoint2(mousePosF);
                        }
                    } else if (tool == "CIRCLE") {
                        auto* circle = dynamic_cast<CircleShape*>(currentShape.get());
                        if (circle) {
                            sf::Vector2f center = circle->getCenter();
                            float dx = mousePosF.x - center.x;
                            float dy = mousePosF.y - center.y;
                            float radius = std::sqrt(dx * dx + dy * dy);
                            circle->setRadius(radius);
                        }
                    }
                }
            }

            // Mouse button released events
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (isDrawing) {
                        isDrawing = false;
                        if (currentShape) {
                            canvas.addShape(std::move(currentShape));
                        }
                    }
                }
            }

            // Text input keyboard inputs (typing letters directly)
            if (event.type == sf::Event::TextEntered && isTyping) {
                // Backspace logic
                if (event.text.unicode == 8) {
                    if (!activeTextString.empty()) {
                        activeTextString.pop_back();
                    }
                }
                // Enter or Escape keys finalize text insertion
                else if (event.text.unicode == 13 || event.text.unicode == 27) {
                    if (!activeTextString.empty()) {
                        canvas.addShape(std::make_unique<TextShape>(
                            menu.getActiveColor(), 0.f, textPosition, activeTextString, 24
                        ));
                    }
                    isTyping = false;
                    activeTextString = "";
                }
                // Standard readable ASCII character input
                else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                    activeTextString += static_cast<char>(event.text.unicode);
                }
            }

            // Global Hotkeys & Shortcuts
            if (event.type == sf::Event::KeyPressed) {
                // Control Combinations
                if (event.key.control) {
                    if (event.key.code == sf::Keyboard::Z) {
                        canvas.undo();
                    } else if (event.key.code == sf::Keyboard::Y) {
                        canvas.redo();
                    } else if (event.key.code == sf::Keyboard::S) {
                        canvas.saveToFile("drawing_project.paint");
                        std::cout << "Project saved successfully via Ctrl+S shortcut.\n";
                    } else if (event.key.code == sf::Keyboard::O) {
                        if (canvas.loadFromFile("drawing_project.paint")) {
                            std::cout << "Project loaded successfully via Ctrl+O shortcut.\n";
                        }
                    }
                }
                // Standard escape cancel hotkey
                else if (event.key.code == sf::Keyboard::Escape) {
                    if (isTyping) {
                        isTyping = false;
                        activeTextString = "";
                    }
                    isDrawing = false;
                    currentShape = nullptr;
                }
            }
        }

        // 6. Update hover highlights continuously based on cursor position
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        menu.update(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));

        // 7. Render frames (Double Buffered)
        window.clear(sf::Color(45, 45, 55)); // Workspace backdrop tone

        // A. Draw canvas sheet and all stored elements
        canvas.draw(window);

        // B. Render active shape preview during dragging
        if (isDrawing && currentShape) {
            currentShape->draw(window);
        }

        // C. Render typing text indicator with elegant cursor "|"
        if (isTyping && fontLoaded) {
            sf::Text previewText;
            previewText.setFont(font);
            previewText.setString(activeTextString + "|");
            previewText.setCharacterSize(24);
            previewText.setFillColor(menu.getActiveColor());
            previewText.setPosition(textPosition);
            window.draw(previewText);
        }

        // D. Draw sidebar menu panel (Rendered last so overlaps don't overflow onto workspace panel)
        menu.draw(window);

        // E. Render buffer to display screen
        window.display();
    }

    return 0;
}
