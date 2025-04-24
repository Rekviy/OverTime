#pragma once
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <overtime.h>

class themeManager {
public:
	struct themeManager::style {
		style(overtime::ref<overtime::texture2D> texture, float textureSize = 1.0f, glm::vec4 color = glm::vec4(1.0f));
		style(glm::vec4 color = glm::vec4(1.0f));
		glm::vec4 _color;
		overtime::ref<overtime::texture2D> _texture;
		float _textureSize;
	};
	static void init();
	static void shutdown();
	static void add(const std::string& id, themeManager::style& style);
	static const overtime::ref<themeManager::style>& getStyle(const std::string& id);
};

#endif