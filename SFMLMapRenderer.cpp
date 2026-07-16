#include "SFMLMapRenderer.h"
#include <QImage>
#include <cmath>
#include <algorithm>
#include <iostream>

SFMLMapRenderer::SFMLMapRenderer(int width, int height)
    : m_width(width), m_height(height), m_currentAreaId("square") {
    std::cout << "1. SFMLMapRenderer 构造开始" << std::endl;
    try {
        std::cout << "2. 尝试创建纹理..." << std::endl;
        m_texture.create(width, height);
        std::cout << "3. 纹理创建成功" << std::endl;
        loadFont();
        std::cout << "4. loadFont 完成" << std::endl;
        buildLayout();
        std::cout << "5. buildLayout 完成" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "SFMLMapRenderer init failed: " << e.what() << std::endl;
    }
    std::cout << "6. SFMLMapRenderer 构造结束" << std::endl;
}

SFMLMapRenderer::~SFMLMapRenderer() {}

bool SFMLMapRenderer::loadFont() {
    // 尝试加载系统字体（可根据实际路径调整）
    if (m_font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
        return true;
    // 如果失败，尝试 SFML 自带字体（如果存在）
    if (m_font.loadFromFile("C:/SFML/include/SFML/Graphics/Fonts/arial.ttf"))
        return true;
    std::cerr << "Warning: Failed to load font. Text will not display." << std::endl;
    return false;
}

void SFMLMapRenderer::setCurrentArea(const std::string& areaId) {
    m_currentAreaId = areaId;
}

void SFMLMapRenderer::buildLayout() {
    auto areas = Map::getAreas();
    int total = areas.size();
    if (total == 0) return;

    double cx = m_width / 2.0;
    double cy = m_height / 2.0;
    double rx = std::min(m_width, m_height) * 0.35;
    double ry = std::min(m_width, m_height) * 0.25;

    for (int i = 0; i < total; ++i) {
        double angle = 2 * M_PI * i / total - M_PI / 2;
        float x = cx + rx * std::cos(angle);
        float y = cy + ry * std::sin(angle);
        m_nodePositions[areas[i].id] = sf::Vector2f(x, y);
    }
}

void SFMLMapRenderer::drawConnections(sf::RenderTarget& target) {
    auto areas = Map::getAreas();
    sf::VertexArray lines(sf::Lines);
    for (const auto& area : areas) {
        auto fromIt = m_nodePositions.find(area.id);
        if (fromIt == m_nodePositions.end()) continue;
        sf::Vector2f from = fromIt->second;
        for (const std::string& connId : area.connections) {
            auto toIt = m_nodePositions.find(connId);
            if (toIt == m_nodePositions.end()) continue;
            sf::Vector2f to = toIt->second;
            lines.append(sf::Vertex(from, sf::Color(100, 100, 180)));
            lines.append(sf::Vertex(to, sf::Color(100, 100, 180)));
        }
    }
    target.draw(lines);
}

void SFMLMapRenderer::drawNodes(sf::RenderTarget& target) {
    for (auto& pair : m_nodePositions) {
        const std::string& id = pair.first;
        sf::Vector2f pos = pair.second;
        const MapArea* area = Map::getAreaById(id);
        if (!area) continue;

        // 节点圆形
        sf::CircleShape node(30);
        node.setOrigin(30, 30);
        node.setPosition(pos);

        sf::Color fillColor(50, 50, 80);
        if (id == m_currentAreaId) {
            fillColor = sf::Color(80, 200, 80); // 当前区域
        } else {
            auto current = Map::getAreaById(m_currentAreaId);
            if (current) {
                for (const auto& conn : current->connections) {
                    if (conn == id) {
                        fillColor = sf::Color(100, 200, 255); // 可移动
                        break;
                    }
                }
            }
        }
        node.setFillColor(fillColor);
        node.setOutlineThickness(3);
        node.setOutlineColor(sf::Color(200, 200, 255));
        target.draw(node);

        // 文字（名称和类型）
        if (m_font.getInfo().family != "") {
            sf::Text text;
            text.setFont(m_font);
            text.setString(area->name);
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::White);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width/2, bounds.height/2);
            text.setPosition(pos.x, pos.y - 40);
            target.draw(text);

            sf::Text typeText;
            typeText.setFont(m_font);
            typeText.setString(Map::getAreaTypeName(area->type));
            typeText.setCharacterSize(10);
            typeText.setFillColor(sf::Color(180, 180, 220));
            sf::FloatRect tBounds = typeText.getLocalBounds();
            typeText.setOrigin(tBounds.width/2, tBounds.height/2);
            typeText.setPosition(pos.x, pos.y + 40);
            target.draw(typeText);
        }
    }
}

QImage SFMLMapRenderer::render() {
    std::cout << "render() 被调用" << std::endl;
    m_texture.clear(sf::Color(30, 30, 50));
    drawConnections(m_texture);
    drawNodes(m_texture);
    m_texture.display();

    sf::Image image = m_texture.getTexture().copyToImage();
    QImage qimage(image.getPixelsPtr(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
    return qimage.copy();
}

std::string SFMLMapRenderer::hitTest(int mouseX, int mouseY) {
    sf::Vector2f mousePos(mouseX, mouseY);
    for (auto& pair : m_nodePositions) {
        sf::Vector2f nodePos = pair.second;
        float dist = std::sqrt((mousePos.x - nodePos.x)*(mousePos.x - nodePos.x) +
                               (mousePos.y - nodePos.y)*(mousePos.y - nodePos.y));
        if (dist < 35) {
            const std::string& id = pair.first;
            auto current = Map::getAreaById(m_currentAreaId);
            if (current) {
                for (const auto& conn : current->connections) {
                    if (conn == id) return id;
                }
            }
            return ""; // 不可移动
        }
    }
    return "";
}