#ifndef SFMLMAPRENDERER_H
#define SFMLMAPRENDERER_H

#include <SFML/Graphics.hpp>
#include <QImage>
#include <string>
#include <map>
#include <vector>
#include "Map.h"   // 你已有的 Map 数据

class SFMLMapRenderer {
public:
    SFMLMapRenderer(int width, int height);
    ~SFMLMapRenderer();

    // 设置当前所在区域ID（用于高亮）
    void setCurrentArea(const std::string& areaId);

    // 渲染地图到 QImage（供 Qt 显示）
    QImage render();

    // 鼠标点击检测，返回点击到的区域ID（若点击到节点）
    std::string hitTest(int mouseX, int mouseY);

private:
    int m_width, m_height;
    sf::RenderTexture m_texture;
    std::string m_currentAreaId;
    std::map<std::string, sf::Vector2f> m_nodePositions;
    sf::Font m_font;

    void buildLayout();
    void drawConnections(sf::RenderTarget& target);
    void drawNodes(sf::RenderTarget& target);
    void drawLabels(sf::RenderTarget& target);
    bool loadFont();
};

#endif