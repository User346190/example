#ifndef SFMLMAPWINDOW_H
#define SFMLMAPWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SFMLMapWindow {
public:
    SFMLMapWindow(int width, int height);
    ~SFMLMapWindow();
    void setCurrentArea(const std::string& areaId);
    void run();
    void close();
    bool loadTileMap(const std::string& tmxPath);
    std::string getActionResult() const { return m_actionResult; }
    void clearActionResult() { m_actionResult = ""; }

private:
    sf::RenderWindow m_window;
    std::string m_currentAreaId;
    bool m_hasTileMap = false;
    std::map<int, sf::Texture> m_tilesetTextures;
    std::map<int, sf::VertexArray> m_tilesetVertexArrays;
    sf::Font m_font;
    float m_zoomLevel;
    std::string m_actionResult;
    struct Hotspot { std::string name; sf::FloatRect rect; };
    std::vector<Hotspot> m_hotspots;
    sf::FloatRect m_invBtnRect;
    sf::FloatRect m_charBtnRect;

    void drawMap();
    void drawTileMap();
    void drawUI();
    bool loadFont();
};

#endif