#include "SFMLMapWindow.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

SFMLMapWindow::SFMLMapWindow(int width, int height)
    : m_window(sf::VideoMode(width, height), "Map", sf::Style::Close),
      m_currentAreaId("square"), m_hasTileMap(false), m_zoomLevel(1.0f), m_actionResult("") {
    m_window.setFramerateLimit(30);
    if (!m_font.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
        std::cerr << "警告: 无法加载字体" << std::endl;
    }
}

SFMLMapWindow::~SFMLMapWindow() {
    m_window.close();
}

void SFMLMapWindow::setCurrentArea(const std::string& areaId) {
    m_currentAreaId = areaId;
}

bool SFMLMapWindow::loadTileMap(const std::string& tmxPath) {
    tmx::Map map;
    if (!map.load(tmxPath)) {
        std::cerr << "加载地图失败: " << tmxPath << std::endl;
        return false;
    }

    const auto& tilesets = map.getTilesets();
    if (tilesets.empty()) {
        std::cerr << "地图没有图集" << std::endl;
        return false;
    }

    auto mapSize = map.getTileCount();
    auto tileSize = map.getTileSize();

    std::map<int, std::string> gidToImage = {
        {1, "srsSOL (1).gif"},
        {589, "9jJhcs (1).gif"},
        {950, "NhCQ8G (1).gif"},
        {1511, "lM9qMQ (1) (1).png"},
        {2631, "2NrZRf (1).gif"}
    };

    m_tilesetTextures.clear();
    std::string mapDir = tmxPath.substr(0, tmxPath.find_last_of("/\\") + 1);

    for (const auto& tileset : tilesets) {
        int firstGID = tileset.getFirstGID();
        auto it = gidToImage.find(firstGID);
        if (it == gidToImage.end()) {
            std::cerr << "未知图集 firstGID: " << firstGID << std::endl;
            return false;
        }
        std::string imagePath = mapDir + it->second;
        sf::Texture tex;
        if (!tex.loadFromFile(imagePath)) {
            std::cerr << "加载图集纹理失败: " << imagePath << std::endl;
            return false;
        }
        m_tilesetTextures[firstGID] = tex;
    }

    const auto& layers = map.getLayers();
    const tmx::TileLayer* tileLayer = nullptr;
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
            break;
        }
    }
    if (!tileLayer) {
        std::cerr << "没有找到 Tile 图层" << std::endl;
        return false;
    }

    m_tilesetVertexArrays.clear();
    std::map<int, int> actualTileCounts;
    for (const auto& tileset : tilesets) {
        int firstGID = tileset.getFirstGID();
        sf::Texture& tex = m_tilesetTextures[firstGID];
        sf::Vector2u texSize = tex.getSize();
        int cols = texSize.x / tileSize.x;
        int rows = texSize.y / tileSize.y;
        actualTileCounts[firstGID] = cols * rows;
        m_tilesetVertexArrays[firstGID] = sf::VertexArray(sf::Quads);
    }

    const auto& tiles = tileLayer->getTiles();
    for (int y = 0; y < mapSize.y; ++y) {
        for (int x = 0; x < mapSize.x; ++x) {
            int tileId = tiles[y * mapSize.x + x].ID;
            if (tileId == 0) continue;

            int targetGID = 0;
            for (const auto& tileset : tilesets) {
                int firstGID = tileset.getFirstGID();
                int nextGID = firstGID + actualTileCounts[firstGID];
                if (tileId >= firstGID && tileId < nextGID) {
                    targetGID = firstGID;
                    break;
                }
            }
            if (targetGID == 0) continue;

            const auto& tileset = *std::find_if(tilesets.begin(), tilesets.end(),
                [targetGID](const tmx::Tileset& ts) { return ts.getFirstGID() == targetGID; });
            auto tsTileSize = tileset.getTileSize();

            int localId = tileId - targetGID;
            sf::Texture& tex = m_tilesetTextures[targetGID];
            sf::Vector2u texSize = tex.getSize();
            unsigned int cols = texSize.x / tsTileSize.x;
            int tx = localId % cols;
            int ty = localId / cols;

            float left = tx * tsTileSize.x;
            float top = ty * tsTileSize.y;
            float right = left + tsTileSize.x;
            float bottom = top + tsTileSize.y;

            float worldX = x * tileSize.x;
            float worldY = y * tileSize.y;

            sf::Vertex quad[4];
            quad[0].position = sf::Vector2f(worldX, worldY);
            quad[1].position = sf::Vector2f(worldX + tileSize.x, worldY);
            quad[2].position = sf::Vector2f(worldX + tileSize.x, worldY + tileSize.y);
            quad[3].position = sf::Vector2f(worldX, worldY + tileSize.y);

            quad[0].texCoords = sf::Vector2f(left, top);
            quad[1].texCoords = sf::Vector2f(right, top);
            quad[2].texCoords = sf::Vector2f(right, bottom);
            quad[3].texCoords = sf::Vector2f(left, bottom);

            for (int i = 0; i < 4; ++i) quad[i].color = sf::Color::White;
            auto& va = m_tilesetVertexArrays[targetGID];
            va.append(quad[0]);
            va.append(quad[1]);
            va.append(quad[2]);
            va.append(quad[3]);
        }
    }

    m_hotspots.clear();
    m_hotspots.push_back({"boss", sf::FloatRect(0, 100, 500, 650)});      // 左侧：战斗（从y=100开始）
    m_hotspots.push_back({"shop", sf::FloatRect(500, 100, 500, 650)});    // 右侧：商店（从y=100开始）

    // 右上角按钮
    m_invBtnRect = sf::FloatRect(900, 20, 80, 40);
    m_charBtnRect = sf::FloatRect(900, 65, 80, 40);

    m_hasTileMap = true;
    return true;
}

void SFMLMapWindow::drawTileMap() {
    if (!m_hasTileMap) return;
    for (auto& pair : m_tilesetVertexArrays) {
        int firstGID = pair.first;
        sf::VertexArray& va = pair.second;
        if (va.getVertexCount() == 0) continue;
        sf::RenderStates states;
        states.texture = &m_tilesetTextures[firstGID];
        m_window.draw(va, states);
    }
}

void SFMLMapWindow::drawUI() {
    // 背包按钮
    sf::RectangleShape invBtn(sf::Vector2f(100, 45));
    invBtn.setPosition(m_invBtnRect.left, m_invBtnRect.top);
    invBtn.setFillColor(sf::Color(0, 120, 200, 230));
    invBtn.setOutlineThickness(2);
    invBtn.setOutlineColor(sf::Color::White);
    m_window.draw(invBtn);
    sf::Text invText;
    invText.setFont(m_font);
    invText.setString("Bag");
    invText.setCharacterSize(20);
    invText.setFillColor(sf::Color::White);
    invText.setPosition(m_invBtnRect.left + 25, m_invBtnRect.top + 10);
    m_window.draw(invText);

    // 角色按钮
    sf::RectangleShape charBtn(sf::Vector2f(100, 45));
    charBtn.setPosition(m_charBtnRect.left, m_charBtnRect.top);
    charBtn.setFillColor(sf::Color(0, 180, 100, 230));
    charBtn.setOutlineThickness(2);
    charBtn.setOutlineColor(sf::Color::White);
    m_window.draw(charBtn);
    sf::Text charText;
    charText.setFont(m_font);
    charText.setString("Info");
    charText.setCharacterSize(20);
    charText.setFillColor(sf::Color::White);
    charText.setPosition(m_charBtnRect.left + 25, m_charBtnRect.top + 10);
    m_window.draw(charText);
}

void SFMLMapWindow::drawMap() {
    if (!m_hasTileMap) return;

    drawTileMap();

    sf::View originalView = m_window.getView();
    m_window.setView(m_window.getDefaultView());

    // 热区不绘制（仅保留点击检测）

    // 背包按钮
    sf::RectangleShape invBtn(sf::Vector2f(100, 45));
    invBtn.setPosition(m_invBtnRect.left, m_invBtnRect.top);
    invBtn.setFillColor(sf::Color(0, 120, 200, 230));
    invBtn.setOutlineThickness(2);
    invBtn.setOutlineColor(sf::Color::White);
    m_window.draw(invBtn);
    sf::Text invText;
    invText.setFont(m_font);
    invText.setString("Bag");
    invText.setCharacterSize(20);
    invText.setFillColor(sf::Color::White);
    invText.setPosition(m_invBtnRect.left + 25, m_invBtnRect.top + 10);
    m_window.draw(invText);

    // 角色按钮
    sf::RectangleShape charBtn(sf::Vector2f(100, 45));
    charBtn.setPosition(m_charBtnRect.left, m_charBtnRect.top);
    charBtn.setFillColor(sf::Color(0, 180, 100, 230));
    charBtn.setOutlineThickness(2);
    charBtn.setOutlineColor(sf::Color::White);
    m_window.draw(charBtn);
    sf::Text charText;
    charText.setFont(m_font);
    charText.setString("Info");
    charText.setCharacterSize(20);
    charText.setFillColor(sf::Color::White);
    charText.setPosition(m_charBtnRect.left + 25, m_charBtnRect.top + 10);
    m_window.draw(charText);

    m_window.setView(originalView);
}

void SFMLMapWindow::run() {
    const float mapWidth = 50 * 32;
    const float mapHeight = 40 * 32;
    const sf::Vector2u windowSize = m_window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / mapWidth;
    float scaleY = static_cast<float>(windowSize.y) / mapHeight;
    float scale = std::min(scaleX, scaleY) * 0.95f;
    if (scale < 0.1f) scale = 0.1f;
    if (scale > 5.0f) scale = 5.0f;

    sf::View view;
    view.setCenter(mapWidth / 2.0f, mapHeight / 2.0f);
    view.setSize(mapWidth, mapHeight);
    view.zoom(1.0f / scale);
    m_window.setView(view);
    m_zoomLevel = scale;

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                return;
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    m_zoomLevel *= 1.1f;
                } else {
                    m_zoomLevel /= 1.1f;
                }
                if (m_zoomLevel < 0.1f) m_zoomLevel = 0.1f;
                if (m_zoomLevel > 5.0f) m_zoomLevel = 5.0f;
                view.setCenter(mapWidth / 2.0f, mapHeight / 2.0f);
                view.setSize(mapWidth, mapHeight);
                view.zoom(1.0f / m_zoomLevel);
                m_window.setView(view);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mx = event.mouseButton.x;
                int my = event.mouseButton.y;

                // 1. 先检测右上角按钮
                if (m_invBtnRect.contains(mx, my)) {
                    m_actionResult = "inventory";
                    m_window.close();
                    return;
                }
                if (m_charBtnRect.contains(mx, my)) {
                    m_actionResult = "character";
                    m_window.close();
                    return;
                }

                // 2. 再检测热区
                for (const auto& hs : m_hotspots) {
                    if (hs.rect.contains(mx, my)) {
                        m_actionResult = hs.name;
                        std::cout << "触发: " << hs.name << std::endl;
                        m_window.close();
                        return;
                    }
                }
            }
        }

        m_window.clear(sf::Color(30, 30, 50));
        drawMap();
        m_window.display();
    }
}

void SFMLMapWindow::close() {
    m_window.close();
}