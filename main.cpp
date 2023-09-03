#include "draw.h"
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE + 3 * 2 * CHARACTER_HEIGHT),
                            "Klipper 16x4 Display Design Tool by jerrymk");

    std::vector<std::vector<bool>> pixelDisplay(DISPLAY_HEIGHT, std::vector<bool>(DISPLAY_WIDTH, false));
    std::vector<std::vector<char>> charDisplay(4, std::vector<char>(16, '\0'));

    while (window.isOpen()) {
        int onHoverX = sf::Mouse::getPosition(window).x / PIXEL_SIZE;
        int onHoverY = sf::Mouse::getPosition(window).y / PIXEL_SIZE;
        int onHoverR = onHoverY / CHARACTER_HEIGHT;
        int onHoverC = onHoverX / CHARACTER_WIDTH;
        static int mouseMode = 0;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
                if (pixelDisplay[onHoverY][onHoverX])
                    mouseMode = -1;
                else
                    mouseMode = 1;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseMode = 0;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Delete) {
                charDisplay[onHoverR][onHoverC] = '\0';
                for (int xi = 0; xi < CHARACTER_WIDTH; xi++)
                    for (int yi = 0; yi < CHARACTER_HEIGHT; yi++)
                        pixelDisplay[onHoverR * CHARACTER_HEIGHT + yi][onHoverC * CHARACTER_WIDTH + xi] = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::T) {
                std::string str;
                std::cout << "Insert text: ";
                std::cin >> str;
                int x = onHoverX / CHARACTER_WIDTH;
                for (auto c: str)
                    charDisplay[onHoverY / CHARACTER_HEIGHT][x++] = c;
                /// TODO: clear pixel display
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::G) {
                std::string str;
                std::vector<std::string> glyphBuffer;
                std::cout << "Insert glyph: (16x16)\n";
                for (int r = 0; r < 16; r++) {
                    std::cin >> str;
                    if (str.size() != 16) {
                        std::cout << "  Glyph insert cancelled\n";
                        break;
                    }
                    glyphBuffer.emplace_back(str);
                }
                if (glyphBuffer.size() == 16) {
                    int y = onHoverR * CHARACTER_HEIGHT;
                    for (auto& r: glyphBuffer) {
                        int x = onHoverC * CHARACTER_WIDTH;
                        for (auto c: r) {
                            if (c == '*') {
                                pixelDisplay[y][x] = 1;
                                charDisplay[y / CHARACTER_HEIGHT][x / CHARACTER_WIDTH] = '\0';
                            } else {
                                pixelDisplay[y][x] = 0;
                                charDisplay[y / CHARACTER_HEIGHT][x / CHARACTER_WIDTH] = '\0';
                            }
                            x++;
                        }
                        y++;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::C && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                    std::cout << "Enter display group name: ";
                    std::string groupName;
                    std::cin >> groupName;
                    std::string output = "# ******************************************************************************\n"
                                         "# *  Generated display group with Klipper 16x4 Display Design Tool by jerrymk  *\n"
                                         "# ******************************************************************************\n\n\n";

                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 16; c += 2) {
                            output += "[display_glyph " + groupName + "_" + char('0' + r) + "_" + char('0' + c / 2) + "]\n";
                            std::string glyph = "data:\n  ";
                            for (int y = r * CHARACTER_HEIGHT; y < r * CHARACTER_HEIGHT + 16; y++) {
                                for (int x = c * CHARACTER_WIDTH; x < c * CHARACTER_WIDTH + 16; x++)
                                    glyph.push_back(pixelDisplay[y][x] ? '*' : '.');
                                glyph.push_back('\n');
                                glyph.push_back(' ');
                                glyph.push_back(' ');
                            }
                            output += glyph + "\n\n";
                        }
                    }

                    for (int r = 0; r < 4; r++) {
                        output += "[display_data " + groupName + " _c" + char('0' + r) + "]\n"
                                                                                         "position: " + char('0' + r) + ", 0\ntext: ";
                        for (int c = 0; c < 8; c++)
                            output += "~" + groupName + "_" + char('0' + r) + "_" + char('0' + c) + "~";

                        output += "\n\n";
                    }

                    std::cout << "Copied to clipboard!\n";
                    sf::Clipboard().setString(output);
                } else {
                    std::string glyph = "data:\n  ";
                    for (int y = onHoverR * CHARACTER_HEIGHT; y < onHoverR * CHARACTER_HEIGHT + 16; y++) {
                        for (int x = onHoverC * CHARACTER_WIDTH; x < onHoverC * CHARACTER_WIDTH + 16; x++)
                            glyph.push_back(pixelDisplay[y][x] ? '*' : '.');
                        glyph.push_back('\n');
                        glyph.push_back(' ');
                        glyph.push_back(' ');
                    }
                    std::cout << "Copied to clipboard!\n";
                    sf::Clipboard().setString(glyph);
                }
            }
        }


        if (mouseMode == 1) {
            pixelDisplay[onHoverY][onHoverX] = 1;
            charDisplay[onHoverY / CHARACTER_HEIGHT][onHoverX / CHARACTER_WIDTH] = '\0';
        }
        if (mouseMode == -1) {
            pixelDisplay[onHoverY][onHoverX] = 0;
            charDisplay[onHoverY / CHARACTER_HEIGHT][onHoverX / CHARACTER_WIDTH] = '\0';
        }



        window.clear(sf::Color::Black);

        sf::VertexArray va(sf::Quads);
        // grid
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 16; c++)
                drawRect(va, c * CHARACTER_WIDTH, r * CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT,
                         ((r + c) & 1 ? 0x2b2b2bff : 0x1a1a1aff), PIXEL_SIZE);
        // char
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 16; c++)
                drawChar(va, charDisplay[r][c], c * CHARACTER_WIDTH + DISPLAY_OFFSET, r * CHARACTER_HEIGHT, 0xfafdffff, PIXEL_SIZE);

        // pixel
        for (int r = 0; r < DISPLAY_HEIGHT; r++)
            for (int c = 0; c < DISPLAY_WIDTH; c++)
                if (pixelDisplay[r][c])
                    drawRect(va, c, r, 1, 1, 0xf5f9ffff, PIXEL_SIZE);


        if (onHoverR < 4) {
            drawRect(va, onHoverC * CHARACTER_WIDTH, onHoverR * CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, 0xff1f1f22);
            drawRect(va, onHoverX, onHoverY, 1, 1, 0xff000088);
        }

        drawString(va, " LEFT: draw pixel                         DEL: clear char", 2, PIXEL_SIZE * DISPLAY_HEIGHT / 2 + CHARACTER_HEIGHT * 0, 0xdbdbdbff, 2,
                   0);
        drawString(va, "    T: insert text                          G: insert glyph", 2, PIXEL_SIZE * DISPLAY_HEIGHT / 2 + CHARACTER_HEIGHT * 1, 0xdbdbdbff, 2,
                   0);
        drawString(va, "CtrlC: copy glyph to clipboard     CtrlShiftC: copy display group to clipboard", 2,
                   PIXEL_SIZE * DISPLAY_HEIGHT / 2 + CHARACTER_HEIGHT * 2, 0xdbdbdbff, 2, 0);

        window.draw(va);
        window.display();
    }

    return 0;
}