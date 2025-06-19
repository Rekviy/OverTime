#pragma once
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <overtime.h>
struct style {
	style(overtime::ref<overtime::texture2D> texture, float textureSize = 1.0f, glm::vec4 color = glm::vec4(1.0f));
	style(glm::vec4 color = glm::vec4(1.0f));
	glm::vec4 _color;
	overtime::ref<overtime::texture2D> _texture;
	float _textureSize;
};
class themeManager {
public:
	
	static void init();
	static void shutdown();
	static void add(const std::string& id, style& newStyle);
	static const overtime::ref<style> getStyle(const std::string& id);
};
#endif